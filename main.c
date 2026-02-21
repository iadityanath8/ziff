#include "logger.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define LINE_BUFF 1811
#define get(x,y,_c) (x)*(_c) + (y) 
#define unimplemented(x) assert(false && x)
#define max(x,y) (x) > (y) ? (x) : (y)
#define unused(x) (void)x

#define ALLOC(nmemb,type) (type*)malloc(sizeof(type)*nmemb)
#define da_append(xs, x)                                                             \
  do {                                                                             \
    if ((xs)->count >= (xs)->capacity) {                                         \
      if ((xs)->capacity == 0) (xs)->capacity = 256;                           \
      else (xs)->capacity *= 2;                                                \
      (xs)->items = realloc((xs)->items, (xs)->capacity*sizeof(*(xs)->items)); \
    }                                                                            \
    \
    (xs)->items[(xs)->count++] = (x);                                            \
  } while (0)

typedef char* str;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


typedef struct {
  str* items;
  i32 count;
  i32 capacity;
}Lines;

typedef struct {
  str* items;
  i32 count;
  i32 capacity;
}Stack;

static inline void free_st(Stack* st) {
  for (i32 i = 0;i < st->count;i++) {
    free(st->items[i]);
  }
  free(st->items);
}

static inline void free_lines(Lines* l) {
  for (i32 i = 0;i < l->count;i++) {
    free(l->items[i]);
  }
  free(l->items);
}

/**
 *
 *   a = "ibkd"
 *   b = "abcd"
 *    
 *     ""  i  b  k  d 
 *  "" 0   0  0  0  0  
 *  a  0   0  0  0  0
 *  b  0   0  1  1  1
 *  c  0   0  1  1  1
 *  d  0   0  1  1  2 
 *
 */
u32 dfs_(str a,str b, u32 i,u32 j,u32 n,u32 m) {
  if (i >= n || j >= m) return 0;
  if (a[i] == b[j]) return 1 + dfs_(a,b,i + 1,j + 1,n,m);
  else {
    return max(dfs_(a,b,i + 1,j,n,m),dfs_(a,b,i,j + 1,n,m));
  }
}

static inline u32 lcs(Lines* a,Lines* b,u32* table) {
  u32 n = a->count;
  u32 _m = b->count;
  u32 m = _m + 1;
  memset(table,0,sizeof(u32)*(n+1)*(m));

  for (u32 i = 1;i <= n;i++) {
    for (u32 j = 1;j <= _m;j++) {
      if (strcmp(a->items[i-1],b->items[j-1]) == 0) {
        table[get(i,j,m)] = 1 + table[get(i-1,j-1,m)];
      } else {
        table[get(i,j,m)] = max(table[get(i-1,j,m)],table[get(i,j-1,m)]); 
      }
    }
  }
  return table[get(n,_m,m)];
}


void print_diff_from_dp(Lines* A, Lines* B) {
  int i = A->count, j = B->count;
  u32 m = j + 1;
  u32* table = ALLOC((i + 1)*(j + 1),u32);
  Stack st = {0};

  char buf[LINE_BUFF];
  lcs(A,B,table);
  while (i > 0 || j > 0) {
    if (i > 0 && j > 0 && strcmp(A->items[i-1],B->items[j-1]) == 0) {
      snprintf(buf,LINE_BUFF,"  %s \n", A->items[i-1]);
      da_append(&st,strdup(buf));
      i--; j--;
    } else if (i > 0 && (j == 0 || table[get(i-1,j,m)] >= table[get(i,j-1,m)])) {
      // deletion
      snprintf(buf,LINE_BUFF,"- %s\n", A->items[i-1]);
      da_append(&st,strdup(buf));
      i--;
    } else if (j > 0 && (i == 0 || table[get(i,j-1,m)] > table[get(i-1,j,m)] )) {
      // insertion
      snprintf(buf,LINE_BUFF,"+ %s\n", B->items[j-1]);
      da_append(&st,strdup(buf));
      j--;
    }
  }

  for (i32 i = st.count - 1;i >= 0;i--) {
    printf("%s",st.items[i]);
  }
  free_st(&st);
  free(table);
}

static inline Lines read_lines(str filename) {
  FILE* fp = fopen(filename,"r");
  if (!fp) log_error("Cannot open file FILE does not exist");
  Lines l = {0};

  char buf[LINE_BUFF];
  while(fgets(buf,LINE_BUFF,fp)) {
    da_append(&l,strdup(buf));
  }
  fclose(fp);
  return l;
}

static inline void apply_patch(Lines *orig, Lines *diff, const char *out_file) {
  FILE *out = fopen(out_file, "w");
  if (!out) {
    perror("fopen");
    return;
  }

  i32 i = 0; 

  for (i32 d = 0; d < diff->count; d++) {
    str line = diff->items[d];

    if (line[0] == ' ' && line[1] == ' ') {
      if (i < orig->count) {
        fputs(orig->items[i], out);
        i++;
      }
    }

    else if (line[0] == '-' && line[1] == ' ') {
      i++;
    }

    else if (line[0] == '+' && line[1] == ' ') {
      fputs(line + 2, out); 
    }
  }

  fclose(out);
}

static inline void usage(const char *prog) {
  printf("Usage:\n");
  printf("  %s diff  <file1> <file2>\n", prog);
  printf("  %s patch <file1> <diff> <out>\n", prog);
  exit(EXIT_FAILURE);
}

i32 main(i32 argc, str argv[]) {

  if (argc < 2)
    usage(argv[0]);

  if (strcmp(argv[1], "diff") == 0) {

    if (argc != 4)
      usage(argv[0]);

    str f1 = argv[2];
    str f2 = argv[3];

    Lines l1 = read_lines(f1);
    Lines l2 = read_lines(f2);

    print_diff_from_dp(&l1, &l2);

    free_lines(&l1);
    free_lines(&l2);

    return 0;
  }

  else if (strcmp(argv[1], "patch") == 0) {

    if (argc != 5)
      usage(argv[0]);

    str orig_file = argv[2];
    str diff_file = argv[3];
    str out_file  = argv[4];

    Lines orig = read_lines(orig_file);
    Lines diff = read_lines(diff_file);

    apply_patch(&orig, &diff, out_file);
    free_lines(&orig);
    free_lines(&diff);
    return 0;
  }
  else {
    usage(argv[0]);
  }

  return 0;
}


