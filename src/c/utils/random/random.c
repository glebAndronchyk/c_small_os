unsigned long seed = 12345;

const unsigned long a = 1664525;
const unsigned long c = 1013904223;
const unsigned long m = 4294967295; // 2^32

unsigned long get_next_random() {
    seed = (a * seed + c) % m;
    return seed;
}

int get_random_in_range(int min, int max) {
    unsigned long randomValue = get_next_random();
    return (randomValue % (max - min + 1)) + min;
}
