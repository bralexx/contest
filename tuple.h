#include <utility>
#include <type_traits>

template<std::size_t, typename T>
struct HeadBase{
    T value;
    HeadBase(){
        value = T();
    }

    HeadBase(const HeadBase&) = default;
    HeadBase(HeadBase&&) noexcept = default;

    template<typename TT>
    explicit HeadBase(TT&& t) : value(std::forward<TT>(t)){}


    static const T& get_value(const HeadBase& t) {
        return (t.value);
    }

    static T& get_value(HeadBase& t) {
        return (t.value);
    }
};

template<std::size_t Ind, typename... Elements>
struct TupleImpl;

template<std::size_t Ind, typename Head, typename... Tail>
struct TupleImpl<Ind, Head, Tail...> : public TupleImpl<Ind + 1, Tail...>, private HeadBase<Ind, Head> {
    typedef TupleImpl<Ind + 1, Tail...> Inherited;
    typedef HeadBase<Ind, Head> Base;

    static constexpr Head& getHead(TupleImpl& t) {return Base::get_value(t);};
    static constexpr const Head& getHead(const TupleImpl& t) {return Base::get_value(t);};
    static constexpr Inherited & getTail(TupleImpl& t) {return t;};
    static constexpr const Inherited & getTail(const TupleImpl& t) {return t;};

    constexpr TupleImpl() : Inherited(), Base() {}

    constexpr TupleImpl(TupleImpl&&)  noexcept = default;

    template<typename OtherHead, typename... OtherTail>
    explicit constexpr TupleImpl(const TupleImpl<Ind, OtherHead, OtherTail...>&& other):
            Inherited(std::move(TupleImpl<Ind, OtherHead, OtherTail...>::getTail(other))),
            Base(std::forward<OtherHead>(TupleImpl<Ind, OtherHead, OtherTail...>::getHead(other))) {}

    TupleImpl& operator=(const TupleImpl& other) {
        getTail(*this) = getTail(other);
        getHead(*this) = getHead(other);
        return *this;
    }

    template<typename... OtherElements>
    TupleImpl& operator=(const TupleImpl<Ind, OtherElements...>& other)
    {
        getHead(*this) = TupleImpl<Ind, OtherElements...>::getHead(other);
        getTail(*this) = TupleImpl<Ind, OtherElements...>::getTail(other);
        return *this;
    }

    TupleImpl& operator=(TupleImpl&& other)  noexcept {
        getTail(*this) = std::move(getTail(other));
        getHead(*this) = std::move(getHead(other));
        return *this;
    }

    explicit constexpr TupleImpl(Head& head, Tail&... tail) :
        Inherited(tail...),
        Base(head) {}

    template<typename THead, typename... TTail>
    explicit constexpr TupleImpl(THead&& thead, TTail&&... ttail) :
        Inherited(std::forward<TTail>(ttail)...),
        Base(std::forward<THead>(thead)) {}

    constexpr TupleImpl(const TupleImpl&) = default;

    template<typename... OtherElements>
    explicit constexpr TupleImpl(const TupleImpl<Ind, OtherElements...>& other):
            Inherited(TupleImpl<Ind, OtherElements...>::getTail(other)),
            Base(TupleImpl<Ind, OtherElements...>::getHead(other)) {}

protected:
    void swap_impl(TupleImpl& other) {
        std::swap(getHead(*this), getHead(other));
        Inherited::swap_impl(getTail(other));
    }
};


template<std::size_t Ind, typename Head>
struct TupleImpl<Ind, Head> : private HeadBase<Ind, Head> {
    typedef HeadBase<Ind, Head> Base;

    static constexpr Head& getHead(TupleImpl& t) {return Base::get_value(t);};
    static constexpr const Head& getHead(const TupleImpl& t) {return Base::get_value(t);};

    constexpr TupleImpl() : Base() {}

    template<typename THead, typename... TTail>
    explicit constexpr TupleImpl(THead&& thead, TTail&&... ttail) :
            Base(std::forward<THead>(thead)) {}

    constexpr TupleImpl(const TupleImpl&) = default;

    template<typename... OtherElements>
    constexpr explicit TupleImpl(const TupleImpl<Ind, OtherElements...>& other):
            Base(TupleImpl<Ind, OtherElements...>::getHead(other)) {}

    constexpr TupleImpl(TupleImpl&&)  noexcept = default;

    template<typename OtherHead, typename... OtherTail>
    explicit constexpr TupleImpl(const TupleImpl<Ind, OtherHead, OtherTail...>&& other):
            Base(std::forward<OtherHead>(TupleImpl<Ind, OtherHead, OtherTail...>::getHead(other))) {}


    template<typename... OtherElements>
    TupleImpl& operator=(const TupleImpl<Ind, OtherElements...>& other)
    {
        getHead(*this) = TupleImpl<Ind, OtherElements...>::getHead(other);
        return *this;
    }

    TupleImpl& operator=(const TupleImpl& other) {
        getHead(*this) = getHead(other);
        return *this;
    }

    TupleImpl& operator=(TupleImpl&& other) {
        getHead(*this) = std::move(getHead(other));
        return *this;
    }

    void swap_impl(TupleImpl& other) {
        std::swap(getHead(*this), getHead(other));
    }
};


template <typename... Elements>
class Tuple: public TupleImpl<0, Elements...> {
    typedef TupleImpl<0, Elements...> Inherited;
public:
    explicit constexpr Tuple(Elements&&... elements): Inherited(std::forward<Elements>(elements)...) {}

    template<typename... Elements2>
    explicit constexpr Tuple(Elements2&&... elements): Inherited(std::forward<Elements2>(elements)...) {}

    constexpr Tuple(const Tuple<Elements...>&) = default;

    template<typename OtherElements>
    constexpr explicit Tuple(const Tuple<OtherElements>& other):Inherited(other) {}


    template<typename OtherElements>
    constexpr explicit Tuple(Tuple<OtherElements>&& other):Inherited(std::move(other)) {}

    Tuple(Tuple&& other) = default;

    Tuple& operator=(const Tuple& other){
        static_cast<Inherited&>(*this) = other;
        return *this;
    }

    Tuple& operator=(Tuple&& other) noexcept {
        static_cast<Inherited&>(*this) = std::move(other);
        return *this;
    }

    template<typename... Elements2>
    Tuple& operator=(Tuple<Elements2...>&& other) noexcept {
        static_cast<Inherited&>(*this) = std::move(other);
        return *this;
    }

    template<typename... Elements2>
    Tuple& operator=(const Tuple<Elements2...>& other) noexcept {
        Inherited::operator=(other);
        return *this;
    }

    void swap(Tuple& other) {
        Inherited::swap_impl(other);
    }
};

template<typename... Elements>
Tuple<typename std::decay<Elements>::type...> makeTuple(Elements&&... elements) {
    typedef Tuple<typename std::decay<Elements>::type...> type;
    return type(std::forward<Elements>(elements)...);
}


//get by index
template<std::size_t Ind, typename Head, typename... Elements>
Head& get(TupleImpl<Ind, Head, Elements...>& t) {
    return (TupleImpl<Ind, Head, Elements...>::getHead(t));
}

template<std::size_t Ind, typename Head, typename... Elements>
const Head& get(const TupleImpl<Ind, Head, Elements...>& t) {
    return (TupleImpl<Ind, Head, Elements...>::getHead(t));
}

template<std::size_t Ind, typename Head, typename... Elements>
Head&& get(TupleImpl<Ind, Head, Elements...>&& t) {
    return std::move((TupleImpl<Ind, Head, Elements...>::getHead(t)));
}

template<std::size_t Ind, typename Head, typename... Elements>
const Head&& get(const TupleImpl<Ind, Head, Elements...>&& t) {
    return std::move((TupleImpl<Ind, Head, Elements...>::getHead(t)));
}

//get by type
template<typename Head, std::size_t Ind, typename... Elements>
Head& get(TupleImpl<Ind, Head, Elements...>& t) {
    return (TupleImpl<Ind, Head, Elements...>::getHead(t));
}

template<typename Head, std::size_t Ind, typename... Elements>
const Head& get(const TupleImpl<Ind, Head, Elements...>& t) {
    return (TupleImpl<Ind, Head, Elements...>::getHead(t));
}

template<typename Head, std::size_t Ind, typename... Elements>
Head&& get(TupleImpl<Ind, Head, Elements...>&& t) {
    return std::move((TupleImpl<Ind, Head, Elements...>::getHead(t)));
}

template<typename Head, std::size_t Ind, typename... Elements>
const Head&& get(const TupleImpl<Ind, Head, Elements...>&& t) {
    return std::move((TupleImpl<Ind, Head, Elements...>::getHead(t)));
}

//tupleCat
template <typename>
struct TupleSize;

template <typename... Elements>
struct TupleSize<Tuple<Elements...>> {
    static constexpr std::size_t size = sizeof...(Elements);
};

template <typename... Elements>
struct TupleSize<Tuple<Elements...>&>: TupleSize<Tuple<Elements...>> {};

template <typename... Elements>
struct TupleSize<Tuple<Elements...>&&>: TupleSize<Tuple<Elements...>> {};

template<typename Indices, typename... Tpls>
struct Concater;

template<std::size_t... Indices, typename Tp1, typename Tp2, typename... Tpls>
struct Concater<std::integer_sequence<std::size_t, Indices...>, Tp1, Tp2, Tpls...> {
    template<typename... Parameters>
    static auto concatTuples(Tp1&& tp1, Tp2&& tp2, Tpls&&... tpls, Parameters&&... parameters) {
        typedef Concater<std::make_index_sequence<TupleSize<Tp2>::size>, Tp2, Tpls...> newConcater;
        return newConcater::concatTuples(std::forward<Tp2>(tp2),
                std::forward<Tpls>(tpls)..., std::forward<Parameters>(parameters)...,
                get<Indices>(std::forward<Tp1>(tp1))...);
    }
};

template<std::size_t... Indices, typename Tp1>
struct Concater<std::integer_sequence<std::size_t, Indices...>, Tp1> {
    template<typename... Parameters>
    static auto concatTuples(Tp1&& tp1, Parameters&&... parameters) {
        return makeTuple(std::forward<Parameters>(parameters)..., get<Indices>(std::forward<Tp1>(tp1))...);
    }
};

template <typename Tp, typename... Tpls>
auto tupleCat(Tp&& tp, Tpls&&... tpls)
{
    return Concater<std::make_index_sequence<TupleSize<Tp>::size>, Tp, Tpls...>
            ::concatTuples(std::forward<Tp>(tp), std::forward<Tpls>(tpls)...);
}

//comp binary operators
template<typename Tp1, typename Tp2, std::size_t i, std::size_t size>
struct TupleComp {
    static constexpr
    bool eq(const Tp1& t1, const Tp2& t2) {
        return get<i>(t1) == get<i>(t2) && TupleComp<Tp1, Tp2, i + 1, size>::eq(t1, t2);
    }

    static constexpr
    bool less(const Tp1& t1, const Tp2& t2) {
        if(get<i>(t1) == get<i>(t2)) {
            return TupleComp<Tp1, Tp2, i + 1, size>::less(t1, t2);
        } else {
            return get<i>(t1) < get<i>(t2);
        }
    }
};

template<typename Tp1, typename Tp2, std::size_t size>
struct TupleComp<Tp1, Tp2, size, size> {
    static constexpr
    bool eq(const Tp1&, const Tp2&) {
        return true;
    }

    static constexpr
    bool less(const Tp1&, const Tp2&) {
        return false;
    }
};

template<typename... Elements1, typename... Elements2>
bool operator==(const Tuple<Elements1...>& t1, const Tuple<Elements2...>& t2) {
    using comp = TupleComp<Tuple<Elements1...>, Tuple<Elements2...>, 0, sizeof...(Elements1)>;
    return comp::eq(t1, t2);
}

template<typename... Elements1, typename... Elements2>
bool operator!=(const Tuple<Elements1...>& t1, const Tuple<Elements2...>& t2) {
    using comp = TupleComp<Tuple<Elements1...>, Tuple<Elements2...>, 0, sizeof...(Elements1)>;
    return !comp::eq(t1, t2);
}

template<typename... Elements1, typename... Elements2>
bool operator<(const Tuple<Elements1...>& t1, const Tuple<Elements2...>& t2) {
    using comp = TupleComp<Tuple<Elements1...>, Tuple<Elements2...>, 0, sizeof...(Elements1)>;
    return comp::less(t1, t2);
}

template<typename... Elements1, typename... Elements2>
bool operator>(const Tuple<Elements1...>& t1, const Tuple<Elements2...>& t2) {
    using comp = TupleComp<Tuple<Elements1...>, Tuple<Elements2...>, 0, sizeof...(Elements1)>;
    return comp::less(t2, t1);
}

template<typename... Elements1, typename... Elements2>
bool operator<=(const Tuple<Elements1...>& t1, const Tuple<Elements2...>& t2) {
    using comp = TupleComp<Tuple<Elements1...>, Tuple<Elements2...>, 0, sizeof...(Elements1)>;
    return !comp::less(t2, t1);
}

template<typename... Elements1, typename... Elements2>
bool operator>=(const Tuple<Elements1...>& t1, const Tuple<Elements2...>& t2) {
    using comp = TupleComp<Tuple<Elements1...>, Tuple<Elements2...>, 0, sizeof...(Elements1)>;
    return !comp::less(t1, t2);
}
