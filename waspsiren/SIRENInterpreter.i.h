#ifndef SIREN_SIRENINTERPRETER_I_H
#define SIREN_SIRENINTERPRETER_I_H

template<class S>
SIRENInterpreter<S>::SIRENInterpreter(std::ostream& err)
    : Interpreter<S>(err), traceLexing(false), traceParsing(false)
{
}

template<class S>
SIRENInterpreter<S>::~SIRENInterpreter()
{
}

template<class S>
bool SIRENInterpreter<S>::parse(std::istream& in,
                                std::size_t startLine,
                                std::size_t startColumn)
{
    return parseStream(in, "selection statement", startLine, startColumn);
}

template<class S>
bool SIRENInterpreter<S>::parseStream(std::istream& in,
                                      const std::string& sname,
                                      std::size_t start_line,
                                      std::size_t start_column)
{
    return Interpreter<S>::template parse_impl<SIRENParser>(
        in, sname, start_line, start_column);
}

template<class S>
bool SIRENInterpreter<S>::parseString(const std::string& input,
                                      const std::string& sname,
                                      std::size_t startLine,
                                      std::size_t startColumn)
{
    std::istringstream iss(input);
    return parseStream(iss, sname, startLine, startColumn);
}

template<class S>
template<typename TAdapter>
std::size_t SIRENInterpreter<S>::evaluate(
    TAdapter& node, SIRENResultSet<TAdapter>& result) const
{
    NodeView selection_root = Interpreter<S>::root();
    if (selection_root.child_count() == 0)
        return 0;

    std::vector<TAdapter> stage;
    evaluate_selection_expression(selection_root.child_at(0), node, stage);
    for (std::size_t i = 0; i < stage.size(); ++i)
        result.push(stage[i]);
    return result.result_count();
}

template<class S>
template<typename TAdapter>
void SIRENInterpreter<S>::evaluate_selection_expression(
    const NodeView& context,
    TAdapter& node,
    std::vector<TAdapter>& stage) const
{
    if (context.type() != wasp::UNION &&
        context.type() != wasp::INTERSECT &&
        context.type() != wasp::EXCEPT)
    {
        evaluate_selection(context, node, stage);
        return;
    }

    std::vector<TAdapter> left;
    std::vector<TAdapter> right;
    evaluate_selection_expression(context.child_at(0), node, left);
    evaluate_selection_expression(context.child_at(2), node, right);

    if (context.type() == wasp::UNION)
    {
        stage = left;
        stage.reserve(left.size() + right.size());
        for (std::size_t i = 0; i < right.size(); ++i)
            if (std::find(stage.begin(), stage.end(), right[i]) == stage.end())
                stage.push_back(right[i]);
    }
    else if (context.type() == wasp::INTERSECT)
    {
        stage.reserve((std::min)(left.size(), right.size()));
        for (std::size_t i = 0; i < left.size(); ++i)
            if (std::find(right.begin(), right.end(), left[i]) != right.end())
                stage.push_back(left[i]);
    }
    else
    {
        stage.reserve(left.size());
        for (std::size_t i = 0; i < left.size(); ++i)
            if (std::find(right.begin(), right.end(), left[i]) == right.end())
                stage.push_back(left[i]);
    }
}

template<class S>
template<typename TAdapter>
void SIRENInterpreter<S>::evaluate_selection(
    const NodeView& context,
    TAdapter& node,
    std::vector<TAdapter>& stage) const
{
    bool root_oriented = context.type() == wasp::DOCUMENT_ROOT;
    if (context.type() == wasp::ANY)
        root_oriented = context.child_count() == 0 ||
                        context.child_at(0).type() == wasp::ANY;

    TAdapter start(node);
    if (root_oriented)
        while (start.has_parent())
            start = start.parent();

    stage.push_back(start);
    if (context.type() == wasp::DOCUMENT_ROOT && context.child_count() == 1)
        return;
    evaluate(context, stage);
}

template<class S>
template<typename TAdapter>
std::size_t SIRENInterpreter<S>::evaluate(
    const NodeView& context,
    std::vector<TAdapter>& stage) const
{
    switch (context.type())
    {
        default:
            stage.clear();
            break;
        case DOCUMENT_ROOT:
            if (context.child_count() > 1)
                evaluate(context.child_at(1), stage);
            break;
        case SEPARATOR:
            break;
        case DECL:
            search_child_name(context, stage);
            break;
        case PARENT:
        {
            std::vector<TAdapter> parents;
            parents.reserve(stage.size());
            for (std::size_t i = 0; i < stage.size(); ++i)
                if (stage[i].has_parent() &&
                    std::find(parents.begin(), parents.end(), stage[i].parent()) ==
                        parents.end())
                    parents.push_back(stage[i].parent());
            stage.swap(parents);
            break;
        }
        case OBJECT:
            evaluate(context.child_at(0), stage);
            if (!stage.empty())
                evaluate(context.child_at(2), stage);
            break;
        case ANY:
        {
            if (context.child_count() == 0)
            {
                recursive_child_select(context, stage);
                break;
            }

            std::size_t right_index = 0;
            if (context.child_at(0).type() == wasp::ANY)
            {
                std::vector<TAdapter> descendants(stage);
                recursive_child_select(context, descendants);
                stage.insert(stage.end(), descendants.begin(), descendants.end());
                right_index = 1;
            }
            else
            {
                evaluate(context.child_at(0), stage);
                if (stage.empty())
                    break;
                if (context.child_count() == 2)
                {
                    recursive_child_select(context, stage);
                    break;
                }
                std::vector<TAdapter> descendants(stage);
                recursive_child_select(context, descendants);
                stage.insert(stage.end(), descendants.begin(), descendants.end());
                right_index = 2;
            }

            if (right_index < context.child_count())
                evaluate(context.child_at(right_index), stage);
            break;
        }
        case FOLLOWING_SIBLING:
            search_siblings(context, stage, true);
            break;
        case PRECEDING_SIBLING:
            search_siblings(context, stage, false);
            break;
        case PREDICATED_CHILD:
        {
            std::vector<TAdapter> input;
            input.swap(stage);
            stage.reserve(input.size());
            NodeView predicate = context.child_at(2);
            if (predicate.type() == wasp::INDEX)
            {
                std::vector<TAdapter> candidates;
                candidates.swap(input);
                evaluate(context.child_at(0), candidates);
                std::size_t start = predicate.child_at(0).to_int();
                std::size_t end = start;
                std::size_t stride = 1;
                if (predicate.child_count() >= 3)
                    end = predicate.child_at(2).to_int();
                if (predicate.child_count() == 5)
                    stride = predicate.child_at(4).to_int();
                if (stride == 0)
                    break;
                for (std::size_t i = 0; i < candidates.size(); ++i)
                {
                    std::size_t position = i + 1;
                    if (position >= start && position <= end &&
                        (position - start) % stride == 0)
                        stage.push_back(candidates[i]);
                }
                break;
            }
            std::vector<TAdapter> candidates;
            candidates.reserve(1);
            for (std::size_t input_i = 0; input_i < input.size(); ++input_i)
            {
                candidates.clear();
                candidates.push_back(input[input_i]);
                evaluate(context.child_at(0), candidates);
                for (std::size_t i = 0; i < candidates.size(); ++i)
                    if (predicate_matches(predicate, candidates[i], i + 1,
                                          candidates.size()))
                        stage.push_back(candidates[i]);
            }
            break;
        }
    }
    return stage.size();
}

template<class S>
template<typename TAdapter>
void SIRENInterpreter<S>::search_child_name(
    const NodeView& context, std::vector<TAdapter>& stage) const
{
    const char* name = context.name();
    const std::size_t input_size = stage.size();
    for (std::size_t i = 0; i < input_size; ++i)
    {
        TAdapter node = stage[i];
        for (auto child = node.begin(); child != node.end(); child.next())
            if (wildcard_string_match(name, child.get().name()))
                stage.push_back(child.get());
    }
    stage.erase(stage.begin(), stage.begin() + input_size);
}

template<class S>
template<typename TAdapter>
void SIRENInterpreter<S>::recursive_child_select(
    const NodeView&, std::vector<TAdapter>& stage) const
{
    std::vector<TAdapter> roots;
    roots.swap(stage);
    std::vector<TAdapter> descendants;
    std::vector<std::pair<TAdapter, bool>> pending;
    pending.reserve(roots.size());
    for (std::size_t i = roots.size(); i > 0; --i)
        pending.push_back(std::make_pair(roots[i - 1], true));

    std::vector<TAdapter> children;

    while (!pending.empty())
    {
        TAdapter current = pending.back().first;
        bool is_root = pending.back().second;
        pending.pop_back();
        if (!is_root)
            descendants.push_back(current);

        children.clear();
        for (auto child = current.begin(); child != current.end(); child.next())
            children.push_back(child.get());
        for (std::size_t i = children.size(); i > 0; --i)
            pending.push_back(std::make_pair(children[i - 1], false));
    }
    stage.swap(descendants);
}

template<class S>
template<typename TAdapter>
void SIRENInterpreter<S>::search_siblings(
    const NodeView& context,
    std::vector<TAdapter>& stage,
    bool following) const
{
    std::vector<TAdapter> matches;
    const char* name = context.child_at(1).name();
    for (std::size_t i = 0; i < stage.size(); ++i)
    {
        if (!stage[i].has_parent())
            continue;
        bool seen = false;
        TAdapter parent = stage[i].parent();
        for (auto sibling = parent.begin(); sibling != parent.end(); sibling.next())
        {
            if (sibling.get() == stage[i])
            {
                seen = true;
                continue;
            }
            bool selected_side = following ? seen : !seen;
            if (selected_side && wildcard_string_match(name, sibling.get().name()))
                matches.push_back(sibling.get());
        }
    }
    stage.swap(matches);
}

template<class S>
bool SIRENInterpreter<S>::expression_number(
    const ExpressionValue& value, double& number)
{
    if (value.kind == ExpressionValue::NUMBER)
    {
        number = value.number;
        return true;
    }
    std::vector<std::string> strings = expression_strings(value);
    if (strings.empty())
        return false;
    char* end = NULL;
    number = std::strtod(strings[0].c_str(), &end);
    return end != strings[0].c_str() && *end == '\0';
}

template<class S>
std::vector<std::string> SIRENInterpreter<S>::expression_strings(
    const ExpressionValue& value)
{
    if (value.kind == ExpressionValue::NODE_SET)
        return value.nodes;
    if (value.kind == ExpressionValue::STRING)
        return std::vector<std::string>(1, value.string);
    if (value.kind == ExpressionValue::BOOLEAN)
        return std::vector<std::string>(1, value.boolean ? "true" : "false");
    std::ostringstream out;
    out << value.number;
    return std::vector<std::string>(1, out.str());
}

template<class S>
bool SIRENInterpreter<S>::expression_boolean(const ExpressionValue& value)
{
    if (value.kind == ExpressionValue::BOOLEAN)
        return value.boolean;
    if (value.kind == ExpressionValue::NUMBER)
        return value.number != 0.0 && !std::isnan(value.number);
    if (value.kind == ExpressionValue::NODE_SET)
        return !value.nodes.empty();
    return !value.string.empty();
}

template<class S>
template<typename TAdapter>
typename SIRENInterpreter<S>::ExpressionValue
SIRENInterpreter<S>::evaluate_expression(
    const NodeView& context,
    TAdapter& node,
    std::size_t position,
    std::size_t size) const
{
    ExpressionValue result;

    if (context.type() == wasp::DECL || context.type() == wasp::VALUE ||
        context.type() == wasp::OBJECT || context.type() == wasp::ANY ||
        context.type() == wasp::PREDICATED_CHILD)
    {
        if (context.type() == wasp::DECL || context.type() == wasp::VALUE)
        {
            std::string raw = context.data();
            bool quoted = raw.size() >= 2 &&
                          ((raw.front() == '\'' && raw.back() == '\'') ||
                           (raw.front() == '"' && raw.back() == '"'));
            std::string text = wasp::strip_quotes(raw);
            if (quoted)
            {
                result.kind = ExpressionValue::STRING;
                result.string = text;
                return result;
            }
            char* end = NULL;
            double number = std::strtod(text.c_str(), &end);
            if (!text.empty() && end != text.c_str() && *end == '\0')
            {
                result.kind = ExpressionValue::NUMBER;
                result.number = number;
                return result;
            }

            if (context.type() == wasp::VALUE)
            {
                result.kind = ExpressionValue::STRING;
                result.string = text;
                return result;
            }

            std::vector<TAdapter> selected(1, node);
            evaluate(context, selected);
            result.kind = ExpressionValue::NODE_SET;
            for (std::size_t i = 0; i < selected.size(); ++i)
                result.nodes.push_back(selected[i].data());
            return result;
        }

        std::vector<TAdapter> selected(1, node);
        evaluate(context, selected);
        result.kind = ExpressionValue::NODE_SET;
        for (std::size_t i = 0; i < selected.size(); ++i)
            result.nodes.push_back(selected[i].data());
        return result;
    }

    if (context.type() == wasp::KEYED_VALUE)
    {
        ExpressionValue left = evaluate_expression(context.child_at(0), node,
                                                   position, size);
        NodeView right_context = context.child_at(2);
        ExpressionValue right = evaluate_expression(right_context, node,
                                                    position, size);
        std::vector<std::string> lhs = expression_strings(left);
        std::vector<std::string> rhs = expression_strings(right);
        bool matched = false;
        std::size_t op = context.child_at(1).type();
        for (std::size_t i = 0; i < lhs.size() && !matched; ++i)
            for (std::size_t j = 0; j < rhs.size() && !matched; ++j)
            {
                char* lend = NULL;
                char* rend = NULL;
                double lnum = std::strtod(lhs[i].c_str(), &lend);
                double rnum = std::strtod(rhs[j].c_str(), &rend);
                bool numeric = lend != lhs[i].c_str() && *lend == '\0' &&
                               rend != rhs[j].c_str() && *rend == '\0';
                int compare = numeric ? (lnum < rnum ? -1 : lnum > rnum ? 1 : 0)
                                      : lhs[i].compare(rhs[j]);
                matched = (op == wasp::EQ && compare == 0) ||
                          (op == wasp::NEQ && compare != 0) ||
                          (op == wasp::LT && compare < 0) ||
                          (op == wasp::LTE && compare <= 0) ||
                          (op == wasp::GT && compare > 0) ||
                          (op == wasp::GTE && compare >= 0);
            }
        result.kind = ExpressionValue::BOOLEAN;
        result.boolean = matched;
        return result;
    }

    if (context.type() == wasp::FUNCTION)
    {
        std::string name = context.name();
        if (name == "position" || name == "last")
        {
            result.kind = ExpressionValue::NUMBER;
            result.number = name == "position" ? position : size;
            return result;
        }

        std::vector<NodeView> arguments;
        if (context.child_count() == 4)
        {
            NodeView argument_node = context.child_at(2);
            if (argument_node.type() == wasp::EXPRESSION)
            {
                arguments.push_back(argument_node.child_at(0));
                arguments.push_back(argument_node.child_at(2));
            }
            else
                arguments.push_back(argument_node);
        }

        if (name == "count" && arguments.size() == 1)
        {
            if (arguments[0].type() == wasp::DECL)
            {
                std::string path = wasp::strip_quotes(arguments[0].data());
                std::size_t count = 0;
                for (auto child = node.begin(); child != node.end(); child.next())
                    if (wildcard_string_match(path.c_str(), child.get().name()))
                        ++count;
                result.kind = ExpressionValue::NUMBER;
                result.number = count;
                return result;
            }
            ExpressionValue value = evaluate_expression(arguments[0], node,
                                                        position, size);
            result.kind = ExpressionValue::NUMBER;
            result.number = value.kind == ExpressionValue::NODE_SET
                                ? value.nodes.size() : 1;
            return result;
        }
        if (name == "not" && arguments.size() == 1)
        {
            result.kind = ExpressionValue::BOOLEAN;
            result.boolean = !predicate_matches(arguments[0], node,
                                                 position, size);
            return result;
        }
        if ((name == "contains" || name == "starts-with") &&
            arguments.size() == 2)
        {
            ExpressionValue left_value;
            if (arguments[0].type() == wasp::DECL)
            {
                std::string path = wasp::strip_quotes(arguments[0].data());
                left_value.kind = ExpressionValue::NODE_SET;
                for (auto child = node.begin(); child != node.end(); child.next())
                    if (wildcard_string_match(path.c_str(), child.get().name()))
                        left_value.nodes.push_back(child.get().data());
            }
            else
                left_value = evaluate_expression(arguments[0], node,
                                                 position, size);
            std::vector<std::string> left = expression_strings(left_value);
            std::vector<std::string> right = expression_strings(
                evaluate_expression(arguments[1], node, position, size));
            result.kind = ExpressionValue::BOOLEAN;
            result.boolean = !left.empty() && !right.empty() &&
                (name == "contains"
                     ? left[0].find(right[0]) != std::string::npos
                     : left[0].compare(0, right[0].size(), right[0]) == 0);
            return result;
        }
        return result;
    }

    if (context.type() == wasp::PARENTHESIS)
        return evaluate_expression(context.child_at(1), node, position, size);

    if (context.child_count() == 2)
    {
        ExpressionValue right = evaluate_expression(context.child_at(1), node,
                                                    position, size);
        if (context.type() == wasp::UNARY_NOT)
        {
            result.kind = ExpressionValue::BOOLEAN;
            result.boolean = !expression_boolean(right);
        }
        else
        {
            double number = 0.0;
            expression_number(right, number);
            result.kind = ExpressionValue::NUMBER;
            result.number = -number;
        }
        return result;
    }

    if (context.child_count() == 3)
    {
        ExpressionValue left = evaluate_expression(context.child_at(0), node,
                                                   position, size);
        std::size_t op = context.type();
        if (op == wasp::WASP_AND || op == wasp::WASP_OR)
        {
            result.kind = ExpressionValue::BOOLEAN;
            bool left_boolean = expression_boolean(left);
            if ((op == wasp::WASP_AND && !left_boolean) ||
                (op == wasp::WASP_OR && left_boolean))
            {
                result.boolean = left_boolean;
                return result;
            }
            result.boolean = expression_boolean(evaluate_expression(
                context.child_at(2), node, position, size));
            return result;
        }
        ExpressionValue right = evaluate_expression(context.child_at(2), node,
                                                    position, size);
        if (op == wasp::EQ || op == wasp::NEQ || op == wasp::LT ||
            op == wasp::LTE || op == wasp::GT || op == wasp::GTE)
        {
            std::vector<std::string> lhs = expression_strings(left);
            std::vector<std::string> rhs = expression_strings(right);
            bool matched = false;
            for (std::size_t i = 0; i < lhs.size() && !matched; ++i)
                for (std::size_t j = 0; j < rhs.size() && !matched; ++j)
                {
                    char* lend = NULL;
                    char* rend = NULL;
                    double lnum = std::strtod(lhs[i].c_str(), &lend);
                    double rnum = std::strtod(rhs[j].c_str(), &rend);
                    bool numeric = lend != lhs[i].c_str() && *lend == '\0' &&
                                   rend != rhs[j].c_str() && *rend == '\0';
                    int compare = numeric
                        ? (lnum < rnum ? -1 : lnum > rnum ? 1 : 0)
                        : lhs[i].compare(rhs[j]);
                    matched = (op == wasp::EQ && compare == 0) ||
                              (op == wasp::NEQ && compare != 0) ||
                              (op == wasp::LT && compare < 0) ||
                              (op == wasp::LTE && compare <= 0) ||
                              (op == wasp::GT && compare > 0) ||
                              (op == wasp::GTE && compare >= 0);
                }
            result.kind = ExpressionValue::BOOLEAN;
            result.boolean = matched;
            return result;
        }

        double lhs = 0.0;
        double rhs = 0.0;
        expression_number(left, lhs);
        expression_number(right, rhs);
        result.kind = ExpressionValue::NUMBER;
        if (op == wasp::PLUS)
            result.number = lhs + rhs;
        else if (op == wasp::MINUS)
            result.number = lhs - rhs;
        else if (op == wasp::MULTIPLY)
            result.number = lhs * rhs;
        else if (op == wasp::DIVIDE)
            result.number = lhs / rhs;
        else if (op == wasp::MODULUS)
            result.number = std::fmod(lhs, rhs);
        else if (op == wasp::EXPONENT)
            result.number = std::pow(lhs, rhs);
        return result;
    }

    return result;
}

template<class S>
template<typename TAdapter>
bool SIRENInterpreter<S>::predicate_matches(
    const NodeView& context,
    TAdapter& node,
    std::size_t position,
    std::size_t size) const
{
    if (context.type() == wasp::DECL || context.type() == wasp::OBJECT ||
        context.type() == wasp::ANY ||
        context.type() == wasp::PREDICATED_CHILD ||
        context.type() == wasp::FOLLOWING_SIBLING ||
        context.type() == wasp::PRECEDING_SIBLING ||
        context.type() == wasp::PARENT)
    {
        std::vector<TAdapter> selected(1, node);
        evaluate(context, selected);
        return !selected.empty();
    }
    if (context.type() == wasp::VALUE)
    {
        std::string text = wasp::strip_quotes(context.data());
        char* end = NULL;
        double number = std::strtod(text.c_str(), &end);
        if (!text.empty() && end != text.c_str() && *end == '\0')
            return static_cast<double>(position) == number;
        return !text.empty();
    }
    ExpressionValue value = evaluate_expression(context, node, position, size);
    if (value.kind == ExpressionValue::NUMBER)
        return static_cast<double>(position) == value.number;
    return expression_boolean(value);
}

#endif
