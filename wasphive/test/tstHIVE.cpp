#include "waspson/SONInterpreter.h"
#include "wasphive/HIVE.h"
#include "waspcore/utils.h"
#include "waspcore/TreeNodePool.h"
#include "waspson/SONNodeView.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <sstream>
#include <memory>

#include "wasphive/test/Paths.h"

std::string test_dir = TEST_DIR_ROOT;

using namespace wasp;

struct HIVETest
{
    std::string input_fail_path;
    std::shared_ptr<std::ifstream>
                                           input_fail;  // test failing input for schema to exercise
    std::shared_ptr<DefaultSONInterpreter> input_fail_interpreter;

    std::string input_pass_path;
    std::shared_ptr<std::ifstream>
                                           input_pass;  // test passing input for schema to exercise
    std::shared_ptr<DefaultSONInterpreter> input_pass_interpreter;

    std::string                            schema_path;
    std::shared_ptr<std::ifstream>         schema;  // schema to validate input
    std::shared_ptr<DefaultSONInterpreter> schema_interpreter;

    std::shared_ptr<std::stringstream> output_data;  // expected output
};

bool load_file_as_string(const std::string&                  file_path,
                         std::shared_ptr<std::stringstream>& s)
{
    return load_file(file_path, *s.get());
}

bool load_streams(HIVETest&          t,
                  const std::string& fname,
                  const std::string& pname,
                  const std::string& oname,
                  const std::string& sname)
{
    t.input_fail_path = test_dir + "/inputs/" + fname;
    t.input_fail      = std::make_shared<std::ifstream>(t.input_fail_path);
    bool file_bad     = t.input_fail->bad() || t.input_fail->fail();
    std::cout << " -Loaded fail input :: " << t.input_fail_path << std::endl;
    EXPECT_FALSE(file_bad);

    t.input_pass_path = test_dir + "/inputs/" + pname;
    t.input_pass      = std::make_shared<std::ifstream>(t.input_pass_path);
    file_bad          = t.input_pass->bad() || t.input_pass->fail();
    std::cout << " -Loaded pass input :: " << t.input_pass_path << std::endl;
    EXPECT_FALSE(file_bad);

    {
        std::string output_path = test_dir + "/outputs/" + oname;
        SCOPED_TRACE(output_path);
        t.output_data = std::make_shared<std::stringstream>();
        EXPECT_TRUE(load_file_as_string(output_path, t.output_data));
        file_bad = t.output_data->bad() || t.output_data->fail();
        std::cout << " -Loaded output (gold) :: " << output_path << std::endl;
        EXPECT_FALSE(file_bad);
    }
    t.schema_path = test_dir + "/schemas/" + sname;
    t.schema      = std::make_shared<std::ifstream>(t.schema_path);
    file_bad      = t.schema->bad() || t.schema->fail();
    std::cout << " -Loaded schema :: " << t.schema_path << std::endl;
    EXPECT_FALSE(file_bad);
    return !file_bad;
}
// load abstract syntax trees (dom)
bool load_ast(HIVETest& t)
{
    t.input_fail_interpreter = std::make_shared<DefaultSONInterpreter>();
    bool input_fail_good =
        t.input_fail_interpreter->parseStream(*t.input_fail, t.input_fail_path);

    EXPECT_TRUE(input_fail_good);

    t.input_pass_interpreter = std::make_shared<DefaultSONInterpreter>();
    bool input_pass_good =
        t.input_pass_interpreter->parseStream(*t.input_pass, t.input_pass_path);
    EXPECT_TRUE(input_pass_good);

    t.schema_interpreter = std::make_shared<DefaultSONInterpreter>();
    bool schema_good =
        t.schema_interpreter->parseStream(*t.schema, t.schema_path);
    EXPECT_TRUE(schema_good);

    return schema_good && input_fail_good && input_pass_good;
}

void do_test(const std::string& name)
{
    SCOPED_TRACE(name);
    HIVE     hive;
    HIVETest t;
    ASSERT_TRUE(load_streams(t, name + ".fail.son", name + ".pass.son",
                             name + ".fail.gld", name + ".sch"));
    ASSERT_TRUE(load_ast(t));
    std::vector<std::string> errors;
    SONNodeView              schema_adapter = t.schema_interpreter->root();
    SONNodeView input_fail_adapter          = t.input_fail_interpreter->root();
    SONNodeView input_pass_adapter          = t.input_pass_interpreter->root();
    bool valid = hive.validate(schema_adapter, input_fail_adapter, errors);
    std::string msgs = HIVE::combine(errors);
    EXPECT_FALSE(valid);
    ASSERT_EQ(t.output_data->str(), msgs);
    valid = hive.validate(schema_adapter, input_pass_adapter, errors);
    EXPECT_TRUE(valid);
}

TEST(HIVE, MinOccurs)
{
    do_test("MinOccurs");
}
TEST(HIVE, MaxOccurs)
{
    do_test("MaxOccurs");
}

TEST(HIVE, ValEnums)
{
    do_test("ValEnums");
}

TEST(HIVE, ValType)
{
    do_test("ValType");
}
TEST(HIVE, MinValInc)
{
    do_test("MinValInc");
}
TEST(HIVE, MinValExc)
{
    do_test("MinValExc");
}
TEST(HIVE, MaxValInc)
{
    do_test("MaxValInc");
}
TEST(HIVE, MaxValExc)
{
    do_test("MaxValExc");
}
TEST(HIVE, ChildAtLeastOne)
{
    do_test("ChildAtLeastOne");
}
TEST(HIVE, ChildAtMostOne)
{
    do_test("ChildAtMostOne");
}
TEST(HIVE, ChildCountEqual)
{
    do_test("ChildCountEqual");
}
TEST(HIVE, ChildExactlyOne)
{
    do_test("ChildExactlyOne");
}
TEST(HIVE, ChildUniqueness)
{
    do_test("ChildUniqueness");
}
TEST(HIVE, DecreaseOver)
{
    do_test("DecreaseOver");
}
TEST(HIVE, ExistsIn)
{
    do_test("ExistsIn");
}

/**
 * Exercise advanced SIREN expressions in an ExistsIn rule from schema
 * creation through input validation. The passing input selects allowed values
 * with predicates, functions, result-set difference, wildcard names, and a
 * sibling axis. The failing input verifies that each excluded value produces
 * a diagnostic for the corresponding input element.
 */
TEST(HIVE, ExistsInWithSIRENExpressions)
{
    const std::string schema_text = R"SON(
test{
    definition{ enabled{} name{} }
    group{ member{ name{} } }
    phase1{ name{} }
    phaseA{ name{} }
    phase10{ name{} }
    marker{}
    choice{ name{} }
    filtered{
        value{
            ExistsIn=[
                "../../definition[enabled/value = 'true']/name/value"
            ]
        }
    }
    last_definition{
        value{
            ExistsIn=[
                "../../definition[position() = last()]/name/value"
            ]
        }
    }
    available{
        value{
            ExistsIn=[
                "../../definition/name/value except ../../definition[enabled/value = 'false']/name/value"
            ]
        }
    }
    populated_group{
        value{
            ExistsIn=[
                "../../group[count(member) >= 2]/member/name/value"
            ]
        }
    }
    prefixed{
        value{
            ExistsIn=[
                "../../definition[starts-with(name/value, 'a')]/name/value"
            ]
        }
    }
    single_character_wildcard{
        value{
            ExistsIn=[ "../../phase?/name/value" ]
        }
    }
    following_choice{
        value{
            ExistsIn=[
                "../../marker/following-sibling::choice/name/value"
            ]
        }
    }
}
)SON";

    const std::string passing_input = R"SON(
test{
    definition{ enabled=true name=alpha }
    definition{ enabled=false name=beta }
    group{ member{ name=solo } }
    group{ member{ name=team_a } member{ name=team_b } }
    phase1{ name=first }
    phaseA{ name=lettered }
    phase10{ name=tenth }
    marker=begin
    choice{ name=selected }
    filtered=alpha
    last_definition=beta
    available=alpha
    populated_group=team_b
    prefixed=alpha
    single_character_wildcard=lettered
    following_choice=selected
}
)SON";

    const std::string failing_input = R"SON(
test{
    definition{ enabled=true name=alpha }
    definition{ enabled=false name=beta }
    group{ member{ name=solo } }
    group{ member{ name=team_a } member{ name=team_b } }
    phase1{ name=first }
    phaseA{ name=lettered }
    phase10{ name=tenth }
    marker=begin
    choice{ name=selected }
    filtered=beta
    last_definition=alpha
    available=beta
    populated_group=solo
    prefixed=beta
    single_character_wildcard=tenth
    following_choice=alpha
}
)SON";

    DefaultSONInterpreter schema_interpreter;
    DefaultSONInterpreter passing_input_interpreter;
    DefaultSONInterpreter failing_input_interpreter;
    ASSERT_TRUE(schema_interpreter.parseString(schema_text));
    ASSERT_TRUE(passing_input_interpreter.parseString(passing_input));
    ASSERT_TRUE(failing_input_interpreter.parseString(failing_input));

    HIVE                     hive;
    std::vector<std::string> errors;
    SONNodeView schema = schema_interpreter.root();
    SONNodeView input  = passing_input_interpreter.root();
    EXPECT_TRUE(hive.validate(schema, input, errors)) << HIVE::combine(errors);

    errors.clear();
    input = failing_input_interpreter.root();
    EXPECT_FALSE(hive.validate(schema, input, errors));
    const std::string messages = HIVE::combine(errors);
    EXPECT_NE(std::string::npos, messages.find("filtered value \"beta\""));
    EXPECT_NE(std::string::npos,
              messages.find("last_definition value \"alpha\""));
    EXPECT_NE(std::string::npos, messages.find("available value \"beta\""));
    EXPECT_NE(std::string::npos,
              messages.find("populated_group value \"solo\""));
    EXPECT_NE(std::string::npos, messages.find("prefixed value \"beta\""));
    EXPECT_NE(std::string::npos,
              messages.find("single_character_wildcard value \"tenth\""));
    EXPECT_NE(std::string::npos,
              messages.find("following_choice value \"alpha\""));
}

TEST(HIVE, Extras)
{
    do_test("Extras");
}
TEST(HIVE, IncreaseOver)
{
    do_test("IncreaseOver");
}
TEST(HIVE, NotExistsIn)
{
    do_test("NotExistsIn");
}
TEST(HIVE, SumOver)
{
    do_test("SumOver");
}
TEST(HIVE, SumOverGroup)
{
    do_test("SumOverGroup");
}

TEST(HIVE, UnknownNode)
{
    do_test("UnknownNode");
}

TEST(HIVE, validation_with_imports)
{
    SCOPED_TRACE("imports");
    do_test("imports");
}
