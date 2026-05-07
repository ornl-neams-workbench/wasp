
#include "waspfiller/FillerInterpreter.h"

#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include <vector>
#include <memory>

namespace wasp
{

TEST(FillerTest, components)
{  // filler components
    std::stringstream stream;
    stream << "1 1.0e10 2r 3 4r 5.0 2p3 2p4.0e-0 " << std::endl;
    stream << "1 1.0e10 f3 a3 3s -3s q3 2q3 n2 3n3 b4 3b4 " << std::endl;
    stream << "3i1 6 3i 4 9 2l1 6 3l9 10 3z" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(23, root.child_count());
    wasp::NODE types[] = {wasp::INTEGER,
                          wasp::REAL,
                          wasp::REPEAT,
                          wasp::REPEAT,
                          wasp::ALTERNATE_NODE,
                          wasp::ALTERNATE_NODE,
                          wasp::INTEGER,
                          wasp::REAL,
                          wasp::FILL,
                          wasp::ADDRESS,
                          wasp::MOVE,
                          wasp::MOVE,
                          wasp::PREVIOUS_REPEAT,
                          wasp::PREVIOUS_REPEAT,
                          wasp::PREVIOUS_ALTERNATE,
                          wasp::PREVIOUS_ALTERNATE,
                          wasp::BACK_PREVIOUS,
                          wasp::BACK_PREVIOUS,
                          wasp::LINEAR_INTERPOLATE,
                          wasp::LINEAR_INTERPOLATE,
                          wasp::LOG_INTERPOLATE,
                          wasp::LOG_INTERPOLATE,
                          wasp::REPEAT_ZERO};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
}  // end
TEST(FillerTest, expansion1)
{  // filler components expansion
    std::stringstream stream;
    stream << "1 1.0e10 2r3 2r2.1 2p3 2p-4.22e-0 3z" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(7, root.child_count());
    wasp::NODE types[] = {wasp::INTEGER,    wasp::REAL,      wasp::REPEAT,
                          wasp::REPEAT,     wasp::ALTERNATE_NODE, wasp::ALTERNATE_NODE,
                          wasp::REPEAT_ZERO};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        SCOPED_TRACE(i);
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<double> vector;
    std::vector<double> expected(13);
    expected[0]  = 1;
    expected[1]  = 1.0e10;
    expected[2]  = 3;
    expected[3]  = 3;
    expected[4]  = 2.1;
    expected[5]  = 2.1;
    expected[6]  = 3;
    expected[7]  = -3;
    expected[8]  = -4.22;
    expected[9]  = 4.22;
    expected[10] = 0.0;
    expected[11] = 0.0;
    expected[12] = 0.0;
    ASSERT_EQ(13, interp.fill(vector));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  // end
TEST(FillerTest, expansion2)
{  // filler components expansion
    std::stringstream stream;
    stream << "1 1.0e10 2r3 2r2.1 2p3 2p-4.22e-0 F22.22" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(7, root.child_count());
    wasp::NODE types[] = {wasp::INTEGER, wasp::REAL,      wasp::REPEAT,
                          wasp::REPEAT,  wasp::ALTERNATE_NODE, wasp::ALTERNATE_NODE,
                          wasp::FILL};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<double> vector(15);
    std::vector<double> expected(15);
    expected[0]  = 1;
    expected[1]  = 1.0e10;
    expected[2]  = 3;
    expected[3]  = 3;
    expected[4]  = 2.1;
    expected[5]  = 2.1;
    expected[6]  = 3;
    expected[7]  = -3;
    expected[8]  = -4.22;
    expected[9]  = 4.22;
    expected[10] = 22.22;
    expected[11] = 22.22;
    expected[12] = 22.22;
    expected[13] = 22.22;
    expected[14] = 22.22;
    expected[14] = 22.22;
    ASSERT_EQ(15, interp.fill(vector, 0, vector.size()));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  // end
TEST(FillerTest, expansion3)
{  // filler components expansion with prexisting values
    std::stringstream stream;
    stream << "1 1.0e10 2r3 2r2.1 2p3 2p-4.22e-0 F22.22" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(7, root.child_count());
    wasp::NODE types[] = {wasp::INTEGER, wasp::REAL,      wasp::REPEAT,
                          wasp::REPEAT,  wasp::ALTERNATE_NODE, wasp::ALTERNATE_NODE,
                          wasp::FILL};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<double> vector(17);
    vector[0] = 500;
    vector[1] = 501;
    std::vector<double> expected(17);
    expected[0]  = 500;
    expected[1]  = 501;
    expected[2]  = 1;
    expected[3]  = 1.0e10;
    expected[4]  = 3;
    expected[5]  = 3;
    expected[6]  = 2.1;
    expected[7]  = 2.1;
    expected[8]  = 3;
    expected[9]  = -3;
    expected[10] = -4.22;
    expected[11] = 4.22;
    expected[12] = 22.22;
    expected[13] = 22.22;
    expected[14] = 22.22;
    expected[15] = 22.22;
    expected[16] = 22.22;
    // starting to fill at index 2
    ASSERT_EQ(15, interp.fill(vector, 2, vector.size()));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  // end
TEST(FillerTest, expansion4)
{  // filler components expansion with prexisting values as integers
    std::stringstream stream;
    stream << "1 100000 2r3 2r2.1 2p3 2p-4.22 F22.22" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(7, root.child_count());
    wasp::NODE types[] = {wasp::INTEGER, wasp::INTEGER,   wasp::REPEAT,
                          wasp::REPEAT,  wasp::ALTERNATE_NODE, wasp::ALTERNATE_NODE,
                          wasp::FILL};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<int> vector(17);
    vector[0] = 500;
    vector[1] = 501;
    std::vector<int> expected(17);
    expected[0]  = 500;
    expected[1]  = 501;
    expected[2]  = 1;
    expected[3]  = 1e5;
    expected[4]  = 3;
    expected[5]  = 3;
    expected[6]  = 2;
    expected[7]  = 2;
    expected[8]  = 3;
    expected[9]  = -3;
    expected[10] = -4;
    expected[11] = 4;
    expected[12] = 22;
    expected[13] = 22;
    expected[14] = 22;
    expected[15] = 22;
    expected[16] = 22;
    // starting to fill at index 2
    ASSERT_EQ(15, interp.fill(vector, 2, vector.size()));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  // end
TEST(FillerTest, log_interp)
{  // filler components expansion via logarithmic interpolation
    std::stringstream stream;
    stream << "3L 1 1e4 2L 1e-4 1e-1" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(2, root.child_count());
    wasp::NODE types[] = {wasp::LOG_INTERPOLATE, wasp::LOG_INTERPOLATE};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        SCOPED_TRACE(i);
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<float> vector;

    std::vector<float> expected = {1,    1e1,  1e2,  1e3, 1e4,
                                   1e-4, 1e-3, 1e-2, 1e-1};

    ASSERT_EQ(expected.size(), interp.fill(vector));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_NEAR(expected[i], vector[i], 1e-8);
    }
}  // end logarithmic filler

TEST(FillerTest, log_interp2)
{  // filler components expansion via log interpolation
    std::stringstream stream("2L1 100");
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();

    std::vector<int> vector;

    std::vector<int> expected = {1, 5, 22, 100};

    ASSERT_EQ(expected.size(), interp.fill(vector));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  
TEST(FillerTest, lin_interp)
{  // filler components expansion via linear interpolation
    std::stringstream stream;
    stream << "3i 10 50" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();
    ASSERT_EQ(1, root.child_count());
    wasp::NODE types[] = {wasp::LINEAR_INTERPOLATE};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<int> vector;

    std::vector<int> expected(5);
    expected[0] = 10;
    expected[1] = 20;
    expected[2] = 30;
    expected[3] = 40;
    expected[4] = 50;

    ASSERT_EQ(expected.size(), interp.fill(vector));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  // end of linear interpolation filler  3I 10 50

TEST(FillerTest, lin_interp2)
{  // filler components expansion via linear interpolation
    std::stringstream stream("2i 0 6");
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();

    std::vector<int> vector;

    std::vector<int> expected = {0, 2, 4, 6};

    ASSERT_EQ(expected.size(), interp.fill(vector));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  

TEST(FillerTest, expansion_mixed)
{  // filler components expansion via mixed
    std::stringstream stream;
    stream << "1.0 3i 10.0 50.0 2.0 3l 1 1e4 25.0 2N3 14B2"
           << " 3i 50.0 10.0 2.3" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));

    NodeView                root  = interp.root();
    std::vector<wasp::NODE> types = {
        wasp::REAL,          wasp::LINEAR_INTERPOLATE,
        wasp::REAL,          wasp::LOG_INTERPOLATE,
        wasp::REAL,          wasp::PREVIOUS_ALTERNATE,
        wasp::BACK_PREVIOUS, wasp::LINEAR_INTERPOLATE,
        wasp::REAL};
    ASSERT_EQ(types.size(), root.child_count());
    for (size_t i = 0; i < root.child_count(); i++)
    {
        SCOPED_TRACE(i);
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<float> vector;

    std::vector<float> expected = {1
                                   // linear interpolate
                                   ,
                                   10,
                                   20,
                                   30,
                                   40,
                                   50,
                                   2.0
                                   // log interpolate
                                   ,
                                   1,
                                   1e1,
                                   1e2,
                                   1e3,
                                   1e4,
                                   25
                                   // 2N3 - previous alternating repeat
                                   // (repeat previous 3, 2 times)
                                   ,
                                   25,
                                   1e4,
                                   1e3,
                                   1e3,
                                   1e4,
                                   25
                                   // 14B2 - 14 back (50), repeat next two
                                   // reverted
                                   ,
                                   2.0,
                                   50
                                   // reverse order linear interpolate
                                   ,
                                   50,
                                   40,
                                   30,
                                   20,
                                   10,
                                   2.3};

    ASSERT_EQ(expected.size(), interp.fill(vector));
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}  // end of mixed expansion
TEST(FillerTest, previous_repeat)
{  // filler components via previous repeat, 'iQ j'
    std::stringstream stream;
    stream << "1 2 3 Q 3 2.4 2Q6" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));

    NodeView root = interp.root();
    ASSERT_EQ(6, root.child_count());
    wasp::NODE types[] = {wasp::INTEGER, wasp::INTEGER,
                          wasp::INTEGER, wasp::PREVIOUS_REPEAT,
                          wasp::REAL,    wasp::PREVIOUS_REPEAT};
    for (size_t i = 0; i < root.child_count(); i++)
    {
        SCOPED_TRACE(i);
        NodeView child = (root.child_at(i));
        ASSERT_TRUE(!child.is_null());
        ASSERT_EQ(types[i], child.type());
    }
    std::vector<float> vector;

    std::vector<float> expected  = {1, 2, 3,
                                    // Q 3
                                    1, 2, 3, 2.4,
                                    // 2Q6
                                    2, 3, 1, 2, 3, 2.4, 2, 3, 1, 2, 3, 2.4};
    size_t             fillCount = interp.fill(vector);
    ASSERT_EQ(expected.size(), fillCount);
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}
TEST(FillerTest, fill_address)
{  // filler components via
    // positive number, -1 number, and extraneous trailing ADDRESS
    std::stringstream stream;
    stream << "F1 A2 +8.2 A5 -2 A1" << std::endl;
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();

    std::vector<float> vector(5);

    std::vector<float> expected  = {1, 8.2, 1, 1, -2};
    size_t             fillCount = interp.fill(vector, 0, expected.size());
    interp.dump_diagnostics(std::cout);
    // +2 additional fills to account for post-ADDRESS writes
    ASSERT_EQ(expected.size() + 2, fillCount);
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}

TEST(FillerTest, mixed)
{
    std::stringstream stream("F0 A1 3R1 2P2 A3 2R8 2S 2I1 100");
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();

    std::vector<float> vector(10);
    // F0 - expected  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0|};
    // A1 - expected  = {|0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // 3R1 - expected  = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
    // 2P2 - expected  = {1, 1, 1, 2, -2, |0, 0, 0, 0, 0};
    // A3 - expected  = {1, 1, |1, 2, -2, 0, 0, 0, 0, 0};
    // 2R8 - expected  = {1, 1, 8, 8, |-2, 0, 0, 0, 0, 0};
    // 2S - expected  = {1, 1, 8, 8, -2, 0, |0, 0, 0, 0};
    // 2I1 100 - expected  = {1, 1, 8, 8, -2, 0, 1, 34, 67, 100|};
    std::vector<float> expected  = {1, 1, 8, 8, -2, 0, 1, 34, 67, 100};
    size_t             fillCount = interp.fill(vector, 0, expected.size());
    interp.dump_diagnostics(std::cout);
    // F1  -> 10 
    // 3R1 ->  3
    // 2P2 ->  2
    // 2R8 ->  2
    // 2I1 100 -> 2+2
    ASSERT_EQ(21, fillCount);
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}

// 
TEST(FillerTest, mixed2)
{
    std::stringstream stream("3R1 2S 2R5");
    DefaultFillerInterpreter interp;
    ASSERT_TRUE(interp.parse(stream));
    NodeView root = interp.root();

    std::vector<float> vector(7, 2);
    std::vector<float> expected  = {1, 1, 1, 2, 2, 5, 5};
    size_t             fillCount = interp.fill(vector, 0, expected.size());
    interp.dump_diagnostics(std::cout);

    ASSERT_EQ(5, fillCount);
    ASSERT_EQ(expected.size(), vector.size());
    for (size_t i = 0; i < vector.size(); i++)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(expected[i], vector[i]);
    }
}

void parse_error_test(std::stringstream& stream, std::stringstream& expected)
{
    std::stringstream cerr;
    DefaultFillerInterpreter interp(cerr);
    // emulate parsing text from a parent document
    interp.stream_name() = "prob.inp";
    bool parsed = interp.parse(stream, 5, 3);
    EXPECT_FALSE(parsed);
    std::stringstream errors;
    interp.dump_diagnostics(errors);
    ASSERT_EQ(expected.str(), errors.str());
    ASSERT_EQ(expected.str(), cerr.str());
}
void fill_error_test(std::stringstream& stream, std::stringstream& expected)
{
    std::stringstream cerr;
    DefaultFillerInterpreter interp(cerr);
    // emulate parsing text from a parent document
    interp.stream_name() = "prob.inp";
    bool parsed = interp.parse(stream, 5, 3);
    ASSERT_TRUE(parsed);
    std::vector<int> data;
    interp.fill(data);
    std::stringstream errors;
    interp.dump_diagnostics(errors);
    ASSERT_EQ(expected.str(), errors.str());
    ASSERT_EQ(expected.str(), cerr.str());
}
// 
TEST(FillerTest, error_provenance)
{
    std::stringstream stream("3R1 ) 2R5");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.7 : syntax error, unexpected invalid token.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_log_syntax)
{
    std::stringstream stream("-2L two hundred");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.7 : syntax error, unexpected invalid token, expecting integer or real.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_log_syntax2)
{
    std::stringstream stream("-2L 2 hundred");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.9 : syntax error, unexpected invalid token, expecting integer or real.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_log_count)
{
    std::stringstream stream("-2L2 100");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Log interpolate entry count requires a non-negative integer!\n");
    fill_error_test(stream, expected_errors);
}
TEST(FillerTest, error_log_start)
{
    std::stringstream stream("2L -2 100");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Log interpolate requires positive start and end values!\n");
    fill_error_test(stream, expected_errors);
}
TEST(FillerTest, error_log_end)
{
    std::stringstream stream("2L 2 -100");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Log interpolate requires positive start and end values!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_lin_count_type)
{
    std::stringstream stream("-2.3I2 100");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.7 : syntax error, unexpected linear interpolate.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_lin_count)
{
    std::stringstream stream("-2I2 100");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Linear interpolate entry count requires a positive integer!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_missing_repeat_count)
{
    std::stringstream stream("r3");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3 : syntax error, unexpected repeat.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_missing_repeat_value)
{
    std::stringstream stream("3r");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.5 : syntax error, unexpected end of file, expecting integer or real.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_missing_alternate_count)
{
    std::stringstream stream("p3");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3 : syntax error, unexpected alternate.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_missing_alternate_value)
{
    std::stringstream stream("3p");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.5 : syntax error, unexpected end of file, expecting integer or real.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_missing_fill_value)
{
    std::stringstream stream("f");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.4 : syntax error, unexpected end of file, expecting integer or real.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_missing_address_value)
{
    std::stringstream stream("A");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.4 : syntax error, unexpected end of file, expecting integer.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_missing_move_value)
{
    std::stringstream stream("S");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3 : syntax error, unexpected move.\n");
    parse_error_test(stream, expected_errors);
}

TEST(FillerTest, error_prepeat_count)
{
    std::stringstream stream("-2Q4");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Previous Repeat component must be non-negative!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_prepeat_entries)
{
    std::stringstream stream("2Q3");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Previous Repeat fill statement, 'i Q j', goes out of bounds at -3!\n");
    fill_error_test(stream, expected_errors);
}
TEST(FillerTest, error_prepeat_entries2)
{
    std::stringstream stream("Q1");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.4: Previous Repeat fill statement, 'i Q j', goes out of bounds at -1!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_palternate_count)
{
    std::stringstream stream("-2N4");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Previous Alternating Repeat component must be non-negative!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_palternate_entries)
{
    std::stringstream stream("2N3");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Previous Alternating Repeat fill statement, 'i N j', goes out of bounds at -1!\n");
    fill_error_test(stream, expected_errors);
}
TEST(FillerTest, error_palternate_entries2)
{
    std::stringstream stream("N1");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.4: Previous Alternating Repeat fill statement, 'i N j', goes out of bounds at -1!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_back_count)
{
    std::stringstream stream("-2B4");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Back Previous component must be non-negative!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_back_entries)
{
    std::stringstream stream("2B3");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Back Previous fill statement, 'i B j', goes out of bounds at 0!\n");
    fill_error_test(stream, expected_errors);
}
TEST(FillerTest, error_back_entries2)
{
    std::stringstream stream("B1");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.4: Back Previous fill statement, 'i B j', goes out of bounds at -1!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_zero)
{
    std::stringstream stream("z");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3 : syntax error, unexpected repeat zero.\n");
    parse_error_test(stream, expected_errors);
}
TEST(FillerTest, error_zero_count)
{
    std::stringstream stream("-2z");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.3: Repeat Zero component must be positive!\n");
    fill_error_test(stream, expected_errors);
}
TEST(FillerTest, error_address_count)
{
    std::stringstream stream("A -2");
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.5: Address component must be positive!\n");
    fill_error_test(stream, expected_errors);
}

TEST(FillerTest, error_address_out_of_bounds)
{
    std::stringstream stream("A 2"); // test data array is size 0, so 2 is out of bounds
    SCOPED_TRACE(stream.str());
    std::stringstream expected_errors("prob.inp:5.5: Address statement, 'A i', is out of bounds at 1 compared to data size of 0!\n");
    fill_error_test(stream, expected_errors);
}
}  // namespace wasp
