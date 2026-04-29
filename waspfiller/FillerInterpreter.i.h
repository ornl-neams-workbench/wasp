#ifndef WASP_FILLERINTERPRETER_I_H
#define WASP_FILLERINTERPRETER_I_H

// Integral types → round
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
convert_log_interp_value(double value)
{
    return static_cast<T>(std::round(value));
}

// Floating-point types → no rounding
template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
convert_log_interp_value(double value)
{
    return static_cast<T>(value);
}

template<class S>
FillerInterpreter<S>::FillerInterpreter(std::ostream& err) : Interpreter<S>(err)
{
}
template<class S>
FillerInterpreter<S>::~FillerInterpreter()
{
}
template<class S>
bool FillerInterpreter<S>::parse(std::istream& in,
                                 std::size_t   startLine,
                                 std::size_t   startColumn)
{
    return Interpreter<S>::template parse_impl<FillerParser>(
        in, Interpreter<S>::stream_name(), startLine, startColumn);
}
template<class S>
template<class T>
size_t
FillerInterpreter<S>::fill(std::vector<T>& vector, size_t index, size_t max)
{
    size_t count = 0;
    if (Interpreter<S>::root().is_null())
        return count;
    for (size_t ni = 0; ni < Interpreter<S>::root().child_count(); ++ni)
    {
        NodeView node = Interpreter<S>::root().child_at(ni);

        bool ok = true;
        switch (node.type())
        {
            default:
            case UNKNOWN:
                error(node)
                    << " encountered an unknown filler node! " <<node.data() << " at line " << node.line() <<  " " << node.column() << std::endl;
                break;
            case MOVE:
                // iS move i indices forward
                if (node.child_count() != 1)
                {
                    error(node) << " Move is missing count!"
                                << std::endl;
                    break;
                }
                index += node.child_at(0).to_int();
                // no indices are filled
                break;
            case LOG_INTERPOLATE: {
                // log interpolation consists of n points between start and end
                // log has three children
                // first - count (n)
                // second - the starting bound
                // third - the ending bound
                // NOTE: there will be count+2 values generated                
                int interior_count = node.child_at(0).to_int();

                if (interior_count < 0)
                {
                    error(node.child_at(0))
                        << ": Log interpolate entry count requires a non-negative integer!"
                        << std::endl;
                    break;
                }

                const int interval_count = interior_count + 1;

                T start{};
                to_type(start, node.child_at(1).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(1)) << "failed to convert!" << std::endl;
                    break;
                }

                T end{};
                to_type(end, node.child_at(2).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(2)) << "failed to convert!" << std::endl;
                    break;
                }

                if (start <= T(0) || end <= T(0))
                {
                    error(node)
                        << ": Log interpolate requires positive start and end values!"
                        << std::endl;
                    break;
                }

                const double d_start = static_cast<double>(start);
                const double d_end   = static_cast<double>(end);

                const double coefficient =
                    std::exp(std::log(d_end / d_start) / static_cast<double>(interval_count));

                double value = d_start;

                // first value
                put_data(index++, start, vector);

                // interior values
                for (int i = 0; i < interior_count; ++i)
                {
                    value *= coefficient;
                    put_data(index++, convert_log_interp_value<T>(value), vector);
                }

                // final value: force exact endpoint
                put_data(index++, end, vector);

                count += interior_count + 2;
                break;
            }
            case LINEAR_INTERPOLATE: {
                // linear interpolation consists of n points between start and
                // end linear has three children first - count (n) second - the
                // starting bound third - the ending bound NOTE: there will be
                // count+2 values generated
                if (node.child_count() != 3)
                {
                    error(node) << " Linear interpolate requires 3 components "
                                << std::endl;
                    break;
                }
                if (node.child_at(0).type() != INTEGER)
                {
                    error(node.child_at(0))
                        << ": Linear interpolate entry count requires an integer!"
                        << std::endl;
                    break;
                }
                int binCount = 0;
                binCount     = node.child_at(0).to_int() + 1;
                if (binCount <= 0)
                {
                    error(node.child_at(0))
                        << ": Linear interpolate entry count requires a "
                           "positive integer!"
                        << std::endl;
                    break;
                }
                T start = 0;
                to_type(start, node.child_at(1).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(1))
                        << ": failed to convert!" << std::endl;
                    break;
                }
                T end = 0;
                to_type(end, node.child_at(2).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(2))
                        << ": failed to convert!" << std::endl;
                    break;
                }

                // need to always iterate from smallest magnitude to largest
                // so reverse the loop if |start| > |end|
                // (also, +0.0 converts anything to floating point so we
                // don't get lots of warnings for this code which is inactive
                // anyway for bools, unsigned ints, etc. because of
                // is_floating_point<T>)
                bool reversed = std::is_floating_point<T>() &&
                                std::abs(start + 0.0) > std::abs(end + 0.0);
                if (reversed)
                    std::swap(start, end);

                // first one
                size_t first_index = index;
                put_data(index, start, vector);
                ++index;

                // middle ones
                T delta = (end - start) / binCount;
                T value = start;
                for (int i = 1; i < binCount; ++i, ++index)
                {
                    value += delta;
                    put_data(index, value, vector);
                }

                // last one
                put_data(index, end, vector);
                ++index;
                size_t last_index = index;

                // undo reversal
                if (reversed)
                    std::reverse(vector.begin() + first_index,
                                 vector.begin() + last_index);

                // set array filled count
                count += binCount + 1;  // +1 for last bin (upper bin)
                break;
            }
            case REPEAT: {
                // repeats have two children
                // first - repeat count
                // second - the value to repeat
                if (node.child_count() != 2)
                {
                    error(node) << ": Repeat filler requires 2 components, the "
                                   "repeat count and the value to repeat!"
                                << std::endl;
                    break;
                }
                if (node.child_at(0).type() != INTEGER)
                {
                    error(node.child_at(0))
                        << ": Repeat filler requires an integer repeat "
                           "count!"
                        << std::endl;
                }
                if (node.child_at(1).type() != INTEGER &&
                    node.child_at(1).type() != REAL)
                {
                    error(node.child_at(1))
                        << ": Repeat filler requires an numeric repeat "
                           "value!"
                        << std::endl;
                    break;
                }

                T value = T(0);
                to_type(value, node.child_at(1).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(1))
                        << ": failed to convert!" << std::endl;
                    break;
                }
                for (size_t i = 0; i < node.child_at(0).to_int();
                     i++, index++, count++)
                {
                    put_data(index, value, vector);
                }
                break;
            }
            case PREVIOUS_REPEAT: {
                // i?Qj - repeats the previous j entries i times. The default
                // value of i is 1.
                //  Previous Repeats have 2 children
                //  i : the number of times to repeat
                //  j : the index from which to repeat
                int repeatCount   = 1;
                int previousCount = -1;
                if (node.child_count() > 1)
                {
                    repeatCount = node.child_at(0).to_int();
                    previousCount =
                        node.child_at(node.child_count() - 1).to_int();
                }
                else if (node.child_count() == 1)
                {
                    previousCount = node.child_at(0).to_int();
                }
                else
                {
                    error(node)
                        << ": Previous Repeat fill statement, 'i Q j', is "
                           "of an invalid form!"
                        << std::endl;
                    break;
                }
                
                if (repeatCount < 0)
                {
                    error(node.child_at(0))
                        << ": Previous Repeat component must be non-negative!" << std::endl;
                        break;
                }
                if (previousCount < 0)
                {
                    error(node.child_at(node.child_count() - 1))
                        << ": Previous Repeat component must be non-negative!" << std::endl;
                        break;
                }
                // because the offset from the write head is constant
                // we only need to update the write head and all else follows.
                for (int pri = 0; pri < repeatCount; ++pri)
                {
                    for (int pci = 0; pci < previousCount;
                         ++pci, ++index, ++count)
                    {
                        int vi    = index - previousCount;
                        T   value = T(0);
                        if (vi < 0 || vi >= int(vector.size()))
                        {
                            error(node)
                                << ": Previous Repeat fill statement, 'i Q "
                                   "j', goes out of bounds at " << vi << "!"
                                << std::endl;
                            return count;
                        }
                        value = vector[vi];
                        put_data(index, value, vector);
                    }
                    //
                }

                break;
            }
            case PREVIOUS_ALTERNATE: {
                // i?Nj - repeats the previous j entries i times,
                //        inverting the sequence each time.
                //        The default value of i is 1.
                //  Previous Repeats have 2 children
                //  i : the number of times to repeat
                //  j : the index from which to repeat
                int repeatCount   = 1;
                int previousCount = -1;
                if (node.child_count() > 1)
                {
                    repeatCount = node.child_at(0).to_int();
                    previousCount =
                        node.child_at(node.child_count() - 1).to_int();
                }
                else if (node.child_count() == 1)
                {
                    previousCount = node.child_at(0).to_int();
                }
                else
                {
                    error(node)
                        << ": Previous Alternating Repeat fill statement, "
                           "'i N j', is of an invalid form!"
                        << std::endl;
                    break;
                }
                if (repeatCount < 0)
                {
                    error(node.child_at(0))
                        << ": Previous Alternating Repeat component must be non-negative!" << std::endl;
                        break;
                }
                if (previousCount < 0)
                {
                    error(node.child_at(node.child_count() - 1))
                        << ": Previous Alternating Repeat component must be non-negative!" << std::endl;
                        break;
                }
                int i      = index;
                int offset = 0;
                for (int pri = 0; pri < repeatCount; ++pri)
                {
                    if (pri % 2 == 1)
                    {
                        offset = previousCount;
                    }  // odd pass = original order
                    else
                    {
                        offset = 0;
                    }  // even pass = inverted order
                    // repeat the prior previousCount inverted (odd pass -
                    // original order)
                    for (int pci = 0; pci < previousCount;
                         ++pci, ++index, ++count)
                    {
                        int vi    = i + (offset - pci - 1);
                        T   value = T(0);
                        if (vi < 0 || vi >= int(vector.size()))
                        {
                            error(node)
                                << ": Previous Alternating Repeat fill "
                                   "statement, 'i N j', goes out of bounds at "
                                   << vi << "!" << std::endl;
                            return count;
                        }
                        value = vector[vi];
                        put_data(index, value, vector);
                    }
                }
                break;
            }
            case BACK_PREVIOUS: {
                // i?Bj - back i entries. From that position,
                //        repeat the previous j entries in reverse order.
                //        The default value of i is 1.
                //  Back Previous have 2 children
                //  i : the number to go back from the current write head
                //  j : the number of entries to repeat in reverse order
                int backCount     = 1;
                int numberEntries = 1;
                if (node.child_count() > 1)
                {
                    numberEntries = node.child_at(1).to_int();
                }
                if (node.child_count() >= 1)
                {
                    backCount = node.child_at(0).to_int();
                }
                if (node.child_count() < 1)
                {
                    error(node)
                        << ": Back Previous fill statement, 'i B j', is of "
                           "an invalid form!"
                        << std::endl;
                    break;
                }
                if (backCount < 0)
                {
                    error(node.child_at(0))
                        << ": Back Previous component must be non-negative!" << std::endl;
                        break;
                }
                if (numberEntries < 0)
                {
                    error(node.child_at(node.child_count() - 1))
                        << ": Previous Alternating Repeat component must be non-negative!" << std::endl;
                        break;
                }
                int s = index - backCount;
                int e = s + numberEntries - 1;
                for (; e >= s; --e, ++index, ++count)
                {
                    int vi    = e;
                    T   value = T(0);
                    if (vi < 0 || vi >= int(vector.size()))
                    {
                        error(node) << ": Back Previous fill statement, 'i B j', "
                                       "goes out of bounds at "<< vi 
                                       << "!" << std::endl;
                        break;
                    }
                    value = vector[vi];
                    put_data(index, value, vector);
                }

                break;
            }
            case REPEAT_ZERO: {
                // iz - repeats zero i times.
                //  Repeat zero have 1 child
                //  i : the number of times to repeat
                int repeatCount = 1;
                if (node.child_count() >= 1)
                {
                    repeatCount = node.child_at(0).to_int();
                }
                if (node.child_count() != 1)
                {
                    error(node) << ": Repeat zero fill statement, 'iz', is of an "
                                   "invalid form!"
                                << std::endl;
                    break;
                }
                if (repeatCount < 0)
                {
                    error(node.child_at(0))
                        << ": Repeat Zero component must be positive!" << std::endl;
                        break;
                }
                T value = T(0);
                for (size_t i = 0; i < repeatCount; i++, index++, count++)
                {
                    put_data(index, value, vector);
                }
                break;
            }
            case ALTERNATE: {
                // alternates have two children
                // first - repeat count
                // second - the value to alternate
                if (node.child_count() != 2)
                {
                    error(node)
                        << ": Alternate filler requires 2 components, the "
                           "repeat count and the value to alternate!"
                        << std::endl;
                    break;
                }
                if (node.child_at(0).type() != INTEGER)
                {
                    error(node.child_at(0))
                        << ": Alternate filler requires an integer repeat "
                           "count!"
                        << std::endl;
                    break;
                }
                if (node.child_at(1).type() != INTEGER &&
                    node.child_at(1).type() != REAL)
                {
                    error(node.child_at(1))
                        << ": Alternate filler requires a numeric alternate "
                           "value!"
                        << std::endl;
                    break;
                }

                T value = 0;
                to_type(value, node.child_at(1).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(1))
                        << ": failed to convert!" << std::endl;
                    break;
                }
                for (size_t i = 0; i < node.child_at(0).to_int();
                     i++, index++, count++)
                {
                    put_data(index, value, vector, i % 2 == 1);
                }
                break;
            }
            case ADDRESS: {
                // ADDRESS have single child
                // first - 1-based position index
                if (node.child_count() != 1)
                {
                    error(node) << ": Address requires 1 component, the "
                                   "address index!"
                                << std::endl;
                    break;
                }
                if (node.child_at(0).type() != INTEGER)
                {
                    error(node.child_at(0))
                        << ": Address requires an integer address "
                           "index!"
                        << std::endl;
                    break;
                }
                int moveTo = node.child_at(0).to_int();
                if (moveTo < 1)
                {
                    error(node.child_at(0))
                        << ": Address component must be positive!" << std::endl;
                        break;
                }
                index =  moveTo -
                        1;  // 1-based user address 0-based code address
                
                if (index >= vector.size())
                {
                    error(node.child_at(0)) << ": Address statement, 'A i', "
                                               "is out of bounds at " << index 
                                               << " compared to data size of " << vector.size() 
                                               << "!" << std::endl;
                    return count;
                }
                break;
            }
            case INTEGER:
            case REAL:

                put_data(node, index, vector);
                index++;
                count++;

                break;
            case FILL: {
                if (node.child_count() != 1)
                {
                    error(node)
                        << ": Fill filler requires 1 component, the fill "
                           "value!"
                        << std::endl;
                    break;
                }
                if (max == vector.max_size())
                {
                    error(node) << ": Fill operation is unbounded!" << std::endl;
                    break;
                }
                T value = T(0);
                to_type(value, node.child_at(0).data(), &ok);
                if (!ok)
                {
                    error(node.child_at(0)) << "convert failed!" << std::endl;
                    break;
                }
                for (; index < max && index < vector.size(); index++, count++)
                {
                    vector[index] = value;
                }
                break;
            }
        }  // end of switch on fill node type
    }  // end of loop over fill nodes
    return count;
}  // end of fill

// ------------------------------------------------------------------------- //

template<class S>
template<class T>
bool FillerInterpreter<S>::put_data(size_t          index,
                                    T               value,
                                    std::vector<T>& vector,
                                    bool            invert)
{
    if (vector.size() < index)
    {
        vector.resize(index);
    }
    if (index == vector.size())
    {
        vector.insert(vector.end(), invert ? -value : value);
    }
    else if (index < vector.size())
    {
        vector[index] = invert ? -value : value;
    }
    else
    {
        return false;
    }
    return true;
}
template<class S>
template<class T>
bool FillerInterpreter<S>::put_data(NodeView        node,
                                    size_t          index,
                                    std::vector<T>& vector,
                                    bool            invert)
{
    if (vector.size() < index)
    {
        vector.resize(index);
    }
    bool ok = true;
    T    d  = T(0);
    to_type(d, node.data(), &ok);

    if (!ok)
    {
        error(node) << " failed to convert value at index " << index << " ! ('"
                    << node.data() << "')" << std::endl;
        return false;
    }
    switch (node.type())
    {
        default:
            return false;
        case INTEGER:
        case REAL:
            return put_data(index, d, vector, invert);
            break;
    }
    return true;
}

#endif
