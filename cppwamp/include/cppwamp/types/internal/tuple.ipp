/*------------------------------------------------------------------------------
                Copyright Butterfly Energy Systems 2014-2015.
           Distributed under the Boost Software License, Version 1.0.
              (See accompanying file LICENSE_1_0.txt or copy at
                    http://www.boost.org/LICENSE_1_0.txt)
------------------------------------------------------------------------------*/

namespace wamp
{

namespace internal
{

//------------------------------------------------------------------------------
template <int N, typename...Ts>
int toTupleElement(const Array& array, std::tuple<Ts...>& tuple)
{
    auto& elem = std::get<N>(tuple);
    try
    {
        array.at(N).to(elem);
    }
    catch (const error::Conversion& e)
    {
        std::ostringstream oss;
        oss << e.what() << " (for tuple element #" << N << ")";
        throw error::Conversion(oss.str());
    }
    return 0;
}

template <int N, typename...Ts>
int fromTupleElement(Array& array, const std::tuple<Ts...>& tuple)
{
    const auto& elem = std::get<N>(tuple);
    array.emplace_back(Variant::from(elem));
    return 0;
}

template <typename... Ts, int ...Seq>
void convertToTuple(const Array& array, std::tuple<Ts...>& tuple,
                    internal::IntegerSequence<Seq...>)
{
    if (array.size() != sizeof...(Ts))
        throw error::Conversion("Cannot convert variant array to tuple; "
                                "sizes do not match");
    using swallow = int[]; // Guarantees left-to-right evaluation
    (void)swallow{0, toTupleElement<Seq>(array, tuple)...};
}

template <typename... Ts, int ...Seq>
void convertFromTuple(Array& array, const std::tuple<Ts...>& tuple,
                      internal::IntegerSequence<Seq...>)
{
    array.reserve(sizeof...(Ts));
    using swallow = int[]; // Guarantees left-to-right evaluation
    (void)swallow{0, fromTupleElement<Seq>(array, tuple)...};
}

//------------------------------------------------------------------------------
template <std::size_t N, typename... Ts>
using EnableIfTupleElement = EnableIf<N != sizeof...(Ts)>;

template <std::size_t N, typename... Ts>
using EnableIfTupleEnd = EnableIf<N == sizeof...(Ts)>;

template <std::size_t N = 0, typename... Ts, EnableIfTupleEnd<N, Ts...> = 0>
bool equalsTuple(const Array&, const std::tuple<Ts...>&)
{
    return true;
}

template <std::size_t N = 0, typename... Ts, EnableIfTupleElement<N, Ts...> = 0>
bool equalsTuple(const Array& array, const std::tuple<Ts...>& tuple)
{
    const auto& arrayElem = array.at(N);
    const auto& tupleElem = std::get<N>(tuple);
    bool result = (arrayElem == tupleElem);
    return result && equalsTuple<N+1, Ts...>(array, tuple);
}

//------------------------------------------------------------------------------
template <std::size_t N = 0, typename... Ts, EnableIfTupleEnd<N, Ts...> = 0>
bool notEqualsTuple(const Array&, const std::tuple<Ts...>&)
{
    return false;
}

template <std::size_t N = 0, typename... Ts, EnableIfTupleElement<N, Ts...> = 0>
bool notEqualsTuple(const Array& array, const std::tuple<Ts...>& tuple)
{
    const auto& arrayElem = array.at(N);
    const auto& tupleElem = std::get<N>(tuple);
    bool result = (arrayElem != tupleElem);
    return result || notEqualsTuple<N+1, Ts...>(array, tuple);
}

} // namespace internal


//------------------------------------------------------------------------------
template <typename... Ts>
void toTuple(const wamp::Variant::Array& array, std::tuple<Ts...>& tuple)
{
    using Seq = typename internal::GenIntegerSequence<sizeof...(Ts)>::type;
    convertToTuple(array, tuple, Seq());
}

//------------------------------------------------------------------------------
template <typename... Ts>
wamp::Variant::Array toArray(const std::tuple<Ts...>& tuple)
{
    using Seq = typename internal::GenIntegerSequence<sizeof...(Ts)>::type;
    Array array;
    convertFromTuple(array, tuple, Seq());
    return array;
}

//------------------------------------------------------------------------------
template <typename... Ts>
void convert(FromVariantConverter& conv, std::tuple<Ts...>& tuple)
{
    auto& variant = conv.variant();
    if (!variant.is<Array>())
    {
        throw error::Conversion("Cannot convert variant to tuple; "
                                "the variant is not an array");
    }
    toTuple(variant.as<Array>(), tuple);
}

//------------------------------------------------------------------------------
template <typename... Ts>
void convert(ToVariantConverter& conv, std::tuple<Ts...>& tuple)
{
    conv.variant() = toArray(tuple);
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator==(const Array& array, const std::tuple<Ts...>& tuple)
{
    auto N = std::tuple_size<std::tuple<Ts...>>::value;
    return array.size() == N ? internal::equalsTuple(array, tuple) : false;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator==(const std::tuple<Ts...>& tuple, const Array& array)
{
    return array == tuple;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator!=(const Array& array, const std::tuple<Ts...>& tuple)
{
    auto N = std::tuple_size<std::tuple<Ts...>>::value;
    return array.size() == N ? internal::notEqualsTuple(array, tuple) : true;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator!=(const std::tuple<Ts...>& tuple, const Array& array)
{
    return array != tuple;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator==(const Variant& variant, const std::tuple<Ts...>& tuple)
{
    return variant.is<Array>() && variant.as<Array>() == tuple;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator==(const std::tuple<Ts...>& tuple, const Variant& variant)
{
    return variant == tuple;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator!=(const Variant& variant, const std::tuple<Ts...>& tuple)
{
    return !variant.is<Array>() || variant.as<Array>() != tuple;
}

//------------------------------------------------------------------------------
template <typename... Ts>
bool operator!=(const std::tuple<Ts...>& tuple, const Variant& variant)
{
    return variant != tuple;
}

} // namespace wamp
