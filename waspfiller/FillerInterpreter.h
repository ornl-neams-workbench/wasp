#ifndef WASP_FILLERINTERPRETER_H
#define WASP_FILLERINTERPRETER_H

#include "waspcore/TreeNodePool.h"
#include "waspcore/Interpreter.h"
#include "waspcore/wasp_bug.h"
#include "waspcore/Format.h"
#include "waspfiller/FillerParser.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <map>
#include <type_traits>

#include "waspcore/decl.h"

namespace wasp
{

/**
 * @class FillerInterpreter
 * @brief High-level interface for parsing and executing Filler input streams.
 *
 * The FillerInterpreter coordinates all core components of the Filler system.
 * It instantiates and connects the FillerLexerImpl and FillerParser, feeds the
 * input stream into the lexer, and provides the resulting token stream to the
 * parser for processing.
 *
 * Supported Filler grammar:
 *
 *  - iRj | i*j | i$j
 *      Store value j in the next i positions of the array.
 *
 *  - iPj
 *      Alternately store j and -j in the next i positions.
 *
 *  - Fj
 *      Fill the remainder of the array with value j, starting from the current position.
 *
*  - Aj
 *      Set the current read head to position j. The next inserted value
 *      will be written at position j + 1.
 *
 *  - iS
 *      Shift the current position by i (positive or negative), allowing skips.
 *
 *  - integer / double
 *      Store a numeric value at the current position.
 *
 *  - i?Qj
 *      Repeat the previous j entries i times (default i = 1).
 *
 *  - i?Nj
 *      Repeat the previous j entries i times, inverting the sequence each time
 *      (default i = 1).
 *
 *  - i?Bj
 *      Move back i positions, then repeat the previous j entries in reverse order
 *      (default i = 1).
 *
 *  - iIj k
 *      Generate a linear interpolation between endpoints j and k with i intermediate
 *      values (total of i + 2 points). At least one space must separate j and k.
 *      For integer arrays, (k - j) / (i + 1) must be an integer.
 *
 *  - iLj k
 *      Generate a logarithmic interpolation between endpoints j and k with i
 *      intermediate values (total of i + 2 points). At least one space must
 *      separate j and k.
 *
 *  - iz
 *      Insert i zeros.
 * 
 *  Examples can be seen in tests/tstFiller.cpp
 */
template<class S = TreeNodePool<>>
class WASP_PUBLIC FillerInterpreter : public Interpreter<S>
{
  public:
    FillerInterpreter(std::ostream& error_stream = std::cerr);
    ~FillerInterpreter();

    /**
     * @brief parse parser the given input stream
     * @param input the stream of the input data
     * @param startLine the first line of the input stream
     * @param startColumn the first column of the input stream
     * @return true, iff no input processing errors were encountered
     */
    bool parse(std::istream& input,
               std::size_t   m_start_line   = 1u,
               std::size_t   m_start_column = 1u);

    
    /**
     * Fill the given vector. The fill will start from the starting
     * index and subsequently keep filling until it is done, or reached the end
     * of the vector. If the end of the vector is reach prior to completion of
     * the fill, the vector is resized as needed.
     * @param vector - the vector to populate/add
     * @param start - the index to start populating at (default=0), when
     * start=vector.size(), values are appended
     * @param max - the maximum size to populate. This is required when an
     * unbounded operation is performed such as a fill, where the dimensions
     * are the bounding function.
     * @return size_t - the number of fills, error_diagnostics will have accumulated new errors
     */
    template<class T>
    size_t fill(std::vector<T>& vector, size_t start = 0, size_t max = 0);

  private:
    Diagnostic& error(NodeView n)
    {
        return Interpreter<S>::error_diagnostic() 
        << position(&Interpreter<S>::stream_name(), n.line(), n.column());
    }
    template<class T>
    bool
    put_data(NodeView node, size_t index, std::vector<T>& vector, bool invert = false);

    template<class T>
    static bool
    put_data(size_t index, T value, std::vector<T>& vector, bool invert = false);

};
#include "waspfiller/FillerInterpreter.i.h"
typedef FillerInterpreter<> DefaultFillerInterpreter;
}  // end of namespace
#endif
