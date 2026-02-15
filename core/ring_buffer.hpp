#pragma once
#include <array>

namespace Core
{

    template <typename T, int N>
    class RingBuffer
    {
        std::array<T, N> buf{};
        int idx = 0;
        int count = 0;

    public:
        void push(const T &v)
        {
            buf[idx] = v;
            idx = (idx + 1) % N;
            if (count < N)
                count++;
        }

        int size() const { return count; }

        // ✅ Correct indexing:
        // get(0) = most recent
        // get(1) = previous
        // get(count-1) = oldest
        T get(int i) const
        {
            int pos = idx - 1 - i;
            if (pos < 0)
                pos += N;
            return buf[pos];
        }
    };

}
