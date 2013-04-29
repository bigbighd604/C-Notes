typedef int (*predicate_t)(const void *);

extern void filter(void *base, size_t member_size, size_t nmem_base,
            void **output_data, size_t *nmem_output, predicate_t predicate);
