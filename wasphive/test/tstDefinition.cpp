#include "waspson/SONInterpreter.h"
#include "wasphive/HIVE.h"
#include "wasphive/InputDefinition.h"
#include "waspcore/TreeNodePool.h"
#include "waspcore/utils.h"
#include "waspson/SONNodeView.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include "wasphive/test/Paths.h"

std::string test_dir = TEST_DIR_ROOT;

using namespace wasp;

TEST(HIVE, definition_strided)
{
    std::string schema_path = test_dir + "/schemas/Definition.sch";

    auto schema        = std::make_shared<std::ifstream>(schema_path);
    bool file_bad      = schema->bad() || schema->fail();
    
    EXPECT_FALSE(file_bad);

    auto schema_interpreter = std::make_shared<DefaultSONInterpreter>();
    bool schema_good = schema_interpreter->parseStream(*schema.get(), schema_path);

    std::cout << " -Loaded schema :: " << schema_path << std::endl;
    
    EXPECT_TRUE(schema_good);
    SONNodeView schema_root = schema_interpreter->root();
    std::stringstream definition_errors;
    Definition::SP    definition = std::make_shared<Definition>();
    bool definition_created = HIVE::create_definition(definition.get(), 
                                                        schema_root,
                                                        definition_errors, true);
    std::cout << definition_errors.str();
    ASSERT_TRUE(definition_created);
    ASSERT_TRUE(definition->has("a"));
    auto a = definition->get("a");
    std::vector<std::pair<int, std::string>> aliased_parts = {
                                                            {0, "x"},
                                                            {1, "y"}, 
                                                            {2, "z"},
                                                            {3, "x"},
                                                            {4, "y"},
                                                            {5, "z"},
                                                            {6, "i"},
                                                            {7, "j"}, 
                                                            {8, "k"},
                                                            {9, "i"},
                                                            {10, "j"},
                                                            {11, "k"}
                                                            };

    for (auto part : aliased_parts)
    {
        SCOPED_TRACE(part.first);
        auto part_def = a->get(part.first);
        ASSERT_TRUE(part_def != nullptr);
        EXPECT_EQ(part.second, part_def->name());
    }
}

/**
 * Verify that InputDefinition validates advanced SIREN paths against the
 * schema hierarchy. This covers predicates, descendant and sibling
 * navigation, wildcards, result-set operators, invalid expressions, reuse of
 * a cached expression in different contexts, and traversal of deep and wide
 * schemas.
 */
TEST(HIVE, definition_validates_siren_paths)
{
    std::stringstream schema_text;
    schema_text << "root{"
                << "  item{ enabled{} value{} }"
                << "  alternate{ value{} }"
                << "  deep{";
    const std::size_t depth = 64;
    for (std::size_t i = 0; i < depth; ++i)
        schema_text << "level" << i << "{";
    schema_text << "target{}";
    for (std::size_t i = 0; i < depth; ++i)
        schema_text << "}";
    schema_text << "}";

    const std::size_t width = 256;
    for (std::size_t i = 0; i < width; ++i)
        schema_text << "option" << i << "{ value{} }";
    schema_text << "}";

    DefaultSONInterpreter schema_interpreter;
    ASSERT_TRUE(schema_interpreter.parseString(schema_text.str()));

    std::stringstream output;
    std::stringstream errors;
    SONNodeView schema_root = schema_interpreter.root();
    InputDefinition definition(schema_root, output, errors);
    ASSERT_TRUE(definition.isInitialized()) << errors.str();

    SONNodeView root_definition =
        schema_root.first_non_decorative_child_by_name("root");
    SONNodeView item_definition =
        root_definition.first_non_decorative_child_by_name("item");
    SONNodeView alternate_definition =
        root_definition.first_non_decorative_child_by_name("alternate");
    ASSERT_FALSE(item_definition.is_null());
    ASSERT_FALSE(alternate_definition.is_null());

    // Reuse one parsed expression against different schema contexts.
    EXPECT_TRUE(definition.isValidPath("value", item_definition));
    EXPECT_TRUE(definition.isValidPath("value", alternate_definition));
    EXPECT_TRUE(definition.isValidPath("enabled", item_definition));
    EXPECT_FALSE(definition.isValidPath("enabled", alternate_definition));

    EXPECT_TRUE(definition.isValidPath(
        "/root/item[enabled][value > 0]/value", schema_root));
    EXPECT_TRUE(definition.isValidPath("/root//value", schema_root));
    EXPECT_TRUE(definition.isValidPath("/root//target", schema_root));
    EXPECT_TRUE(definition.isValidPath("/root/*/value", schema_root));
    EXPECT_TRUE(definition.isValidPath("/root/option*/value", schema_root));
    EXPECT_TRUE(definition.isValidPath(
        "/root/item/value | /root/alternate/value", schema_root));
    EXPECT_TRUE(definition.isValidPath(
        "/root/item/value intersect /root//value", schema_root));
    EXPECT_TRUE(definition.isValidPath(
        "/root//value except /root/alternate/value", schema_root));
    EXPECT_TRUE(definition.isValidPath(
        "/root/item/enabled/following-sibling::value", schema_root));
    EXPECT_TRUE(definition.isValidPath(
        "/root/item/value/preceding-sibling::enabled", schema_root));
    EXPECT_TRUE(definition.isValidPath(
        "/root/item[value = ']']/value", schema_root));

    EXPECT_FALSE(definition.isValidPath("/root/item/missing", schema_root));
    EXPECT_FALSE(definition.isValidPath(
        "/root/item/value | /root/missing/value", schema_root));
    EXPECT_FALSE(definition.isValidPath("/root/item[", schema_root));
    EXPECT_FALSE(definition.isValidPath("", schema_root));
}
