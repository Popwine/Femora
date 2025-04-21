#ifndef CONFIG_H
#define CONFIG_H

#ifdef USE_DOUBLE_PRECISION
    using real = double;
#else
    using real = float;
#endif


#endif //CONFIG_H