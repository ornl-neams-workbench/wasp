#include "waspplot/CustomPlotFile.h"
#include "waspplot/CustomPlotSerialization.h"

#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include <vector>
#include <memory>

namespace wasp
{

namespace
{

CustomPlot::Color testColor(int r, int g, int b, int a)
{
    CustomPlot::Color c;
    c.red(r);
    c.green(g);
    c.blue(b);
    c.alpha(a);
    return c;
}

CustomPlot::Font
testFont(const std::string& family, int size, bool bold, bool italic)
{
    CustomPlot::Font f;
    f.family(family);
    f.pointsize(size);
    f.bold(bold);
    f.italic(italic);
    return f;
}

std::shared_ptr<CustomPlot::Graph> createDetailedGraph()
{
    auto g = std::make_shared<CustomPlot::Graph>();
    g->name("Graph A");
    g->keys()                  = {1, 2, 3};
    g->values()                = {10, 20, 30};
    g->keysUncertaintyLow()    = {0.1, 0.1, 0.1};
    g->keysUncertaintyHigh()   = {0.2, 0.2, 0.2};
    g->valuesUncertaintyLow()  = {1, 1, 1};
    g->valuesUncertaintyHigh() = {2, 2, 2};
    g->areaOpacity(42);
    g->errorType(CustomPlot::etBoth);
    g->lineStyle(CustomPlot::lsStepLeft);
    g->lineWeight(3);
    g->scatterShape(CustomPlot::ssTriangleInverted);
    g->scatterSize(6.0);
    return g;
}

std::shared_ptr<CustomPlot::Bar> createBar()
{
    auto b = std::make_shared<CustomPlot::Bar>();
    b->name("Bar B");
    b->keys()   = {4, 5, 6};
    b->values() = {7, 8, 9};
    return b;
}

std::shared_ptr<CustomPlot::ColorMap> createColorMap()
{
    auto c = std::make_shared<CustomPlot::ColorMap>();
    c->name("Heatmap C");
    c->keys()   = {0, 1};
    c->values() = {0, 1};
    c->data()   = {{1.1, 1.2}, {2.1, 2.2}};
    c->gradient(CustomPlot::gpHot);
    return c;
}

CustomPlot createComplexPlot()
{
    CustomPlot plot;

    // Title
    plot.title().text("Advanced Plot");
    plot.title().visible(true);
    plot.title().color() = testColor(100, 150, 200, 255);
    plot.title().font()  = testFont("Verdana", 14, true, false);

    // Legend
    plot.legend().visible(true);
    plot.legend().font() = testFont("Courier", 10, false, true);

    // Axis: set format and range
    plot.x1Axis().label("Time");
    plot.x1Axis().labelColor()    = testColor(0, 0, 0, 255);
    plot.x1Axis().labelFont()     = testFont("Arial", 10, false, false);
    plot.x1Axis().tickLabelFont() = testFont("Arial", 8, false, false);
    plot.x1Axis().rangeMin(0.0);
    plot.x1Axis().rangeMax(10.0);
    plot.x1Axis().scaleType(CustomPlot::stLogarithmic);
    plot.x1Axis().labelType(CustomPlot::ltDateTime);

    // Series
    plot.series().push_back(createDetailedGraph());
    plot.series().push_back(createBar());
    plot.series().push_back(createColorMap());

    return plot;
}

}  // namespace

TEST(CustomPlotSerializationTest, RoundTripComplexPlot)
{
    CustomPlot original = createComplexPlot();

    DataObject serialized = serializeCustomPlot(original);
    auto       restored   = deserializeCustomPlot(serialized);

    // Title and Font
    EXPECT_EQ(restored->title().text(), "Advanced Plot");
    EXPECT_TRUE(restored->title().visible());
    EXPECT_EQ(restored->title().color().red(), 100);
    EXPECT_EQ(restored->title().font().family(), "Verdana");
    EXPECT_TRUE(restored->title().font().bold());

    // Axis formatting
    EXPECT_EQ(restored->x1Axis().label(), "Time");
    EXPECT_EQ(restored->x1Axis().labelFont().pointsize(), 10);
    EXPECT_EQ(restored->x1Axis().rangeMin(), 0.0);
    EXPECT_EQ(restored->x1Axis().rangeMax(), 10.0);
    EXPECT_EQ(restored->x1Axis().scaleType(), CustomPlot::stLogarithmic);
    EXPECT_EQ(restored->x1Axis().labelType(), CustomPlot::ltDateTime);

    // Series
    ASSERT_EQ(restored->series().size(), 3);

    auto graph =
        std::dynamic_pointer_cast<CustomPlot::Graph>(restored->series()[0]);
    ASSERT_TRUE(graph);
    EXPECT_EQ(graph->lineStyle(), CustomPlot::lsStepLeft);
    EXPECT_EQ(graph->lineWeight(), 3u);
    EXPECT_EQ(graph->scatterShape(), CustomPlot::ssTriangleInverted);
    EXPECT_EQ(graph->keysUncertaintyLow().size(), 3);

    auto bar =
        std::dynamic_pointer_cast<CustomPlot::Bar>(restored->series()[1]);
    ASSERT_TRUE(bar);
    EXPECT_EQ(bar->name(), "Bar B");
    EXPECT_EQ(bar->keys().at(1), 5.0);

    auto cmap =
        std::dynamic_pointer_cast<CustomPlot::ColorMap>(restored->series()[2]);
    ASSERT_TRUE(cmap);
    EXPECT_EQ(cmap->gradient(), CustomPlot::gpHot);
    ASSERT_EQ(cmap->data().size(), 2u);
    EXPECT_NEAR(cmap->data()[1][1], 2.2, 1e-6);
}

TEST(CustomPlotSerializationTest, EmptyPlotIsSerializable)
{
    CustomPlot emptyPlot;
    DataObject data   = serializeCustomPlot(emptyPlot);
    auto       loaded = deserializeCustomPlot(data);
    EXPECT_TRUE(loaded->series().empty());
    EXPECT_EQ(loaded->title().text(), "");
    EXPECT_TRUE(loaded->title().visible());
}

TEST(CustomPlotSerializationTest, GraphHandlesEmptyUncertainty)
{
    auto graph = std::make_shared<CustomPlot::Graph>();
    graph->name("Minimal Graph");
    graph->keys()   = {1, 2};
    graph->values() = {3, 4};

    CustomPlot plot;
    plot.series().push_back(graph);

    DataObject data         = serializeCustomPlot(plot);
    auto       roundtripped = deserializeCustomPlot(data);

    ASSERT_EQ(roundtripped->series().size(), 1);
    auto g2 = std::dynamic_pointer_cast<CustomPlot::Graph>(
        roundtripped->series().at(0));
    ASSERT_TRUE(g2);
    EXPECT_EQ(g2->keys().size(), 2);
    EXPECT_TRUE(g2->keysUncertaintyLow().empty());
}
// TODO add gaurds to deserialization logic to use default structures
TEST(CustomPlotSerializationTest, DISABLED_MissingOptionalFieldsHandledGracefully)
{
    DataObject graphObj;
    graphObj["type"]   = "Graph";
    graphObj["name"]   = "Incomplete Graph";
    graphObj["keys"]   = DataArray{1.0, 2.0};
    graphObj["values"] = DataArray{10.0, 20.0};
    // Missing: errorType, lineStyle, scatterShape, uncertainty, etc.

    auto series = deserializeSeries(graphObj);
    ASSERT_TRUE(series);

    auto graph = std::dynamic_pointer_cast<CustomPlot::Graph>(series);
    ASSERT_TRUE(graph);

    EXPECT_EQ(graph->name(), "Incomplete Graph");
    EXPECT_EQ(graph->keys().size(), 2);
    EXPECT_EQ(graph->lineStyle(), CustomPlot::lsNone);  // Default enum
    EXPECT_TRUE(graph->keysUncertaintyLow().empty());   // Defaults
}

}  // namespace wasp
