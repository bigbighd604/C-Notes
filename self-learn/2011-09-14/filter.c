#include <stdlib.h>
#include <string.h>
#include "filter.h"


void filter(void *base, size_t member_size, size_t nmem_base,
            void **output_data, size_t *nmem_output, predicate_t predicate) {
  int base_counter;
  int output_counter = 0;
  *output_data = malloc(member_size * nmem_base);

  for (base_counter = 0; base_counter < nmem_base; base_counter++) {
    if (predicate(base+base_counter*member_size)) {
      memcpy(*output_data + output_counter*member_size,
             base + base_counter*member_size, member_size);
      output_counter++;
    }
  }
  *nmem_output = output_counter;
}
