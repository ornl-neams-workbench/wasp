#ifndef WASPPLOT_CUSTOMPLOTSERIALIZATION_H
#define WASPPLOT_CUSTOMPLOTSERIALIZATION_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "waspcore/decl.h"
#include "waspcore/Object.h"
#include "waspplot/CustomPlotFile.h"

namespace wasp {

/**
 * @brief Serializes a CustomPlot::Color object into a DataObject.
 */
DataObject WASP_PUBLIC serializeColor(const CustomPlot::Color& color);

/**
 * @brief Serializes a CustomPlot::Font object into a DataObject.
 */
DataObject WASP_PUBLIC serializeFont(const CustomPlot::Font& font);

/**
 * @brief Serializes a CustomPlot::Title object into a DataObject.
 */
DataObject WASP_PUBLIC serializeTitle(const CustomPlot::Title& title);

/**
 * @brief Serializes a CustomPlot::Legend object into a DataObject.
 */
DataObject WASP_PUBLIC serializeLegend(const CustomPlot::Legend& legend);

/**
 * @brief Serializes a CustomPlot::Axis object into a DataObject.
 */
DataObject WASP_PUBLIC serializeAxis(const CustomPlot::Axis& axis);

/**
 * @brief Serializes a polymorphic Series object (Graph, Bar, ColorMap).
 */
DataObject WASP_PUBLIC serializeSeries(const std::shared_ptr<CustomPlot::Series>& series);

/**
 * @brief Serializes an entire CustomPlot object into a DataObject.
 */
DataObject WASP_PUBLIC serializeCustomPlot(const CustomPlot& plot);

/**
 * @brief Deserializes a DataObject into a CustomPlot::Color.
 */
CustomPlot::Color WASP_PUBLIC deserializeColor(const DataObject& obj);

/**
 * @brief Deserializes a DataObject into a CustomPlot::Font.
 */
CustomPlot::Font WASP_PUBLIC deserializeFont(const DataObject& obj);

/**
 * @brief Deserializes a DataObject into a CustomPlot::Title.
 */
CustomPlot::Title WASP_PUBLIC deserializeTitle(const DataObject& obj);

/**
 * @brief Deserializes a DataObject into a CustomPlot::Legend.
 */
CustomPlot::Legend WASP_PUBLIC deserializeLegend(const DataObject& obj);

/**
 * @brief Deserializes a DataObject into a CustomPlot::Axis.
 */
CustomPlot::Axis WASP_PUBLIC deserializeAxis(const DataObject& obj);

/**
 * @brief Deserializes a polymorphic Series object based on the \"type\" field.
 */
std::shared_ptr<CustomPlot::Series> WASP_PUBLIC deserializeSeries(const DataObject& obj);

/**
 * @brief Deserializes a complete CustomPlot from a DataObject.
 */
std::shared_ptr<CustomPlot> WASP_PUBLIC deserializeCustomPlot(const DataObject& obj);

} // namespace wasp
#endif  // WASPPLOT_CUSTOMPLOTSERIALIZATION_H