#ifndef QUINNPRIV2_H
#define QUINNPRIV2_H

#include <assert.h>
#include "quinn.h"
#include "bseq.h"
#include "kseq.h"

#define QUINN_PARENT_UNSET (-1)
#define QUINN_PARENT_TMP_PRI (-2)
#define QUINN_DBG_NO_KALLOC 0x1
#define QUINN_DBG_PRINT_QNAME 0x2
#define QUINN_DBG_PRINT_SEED 0x4
#define QUINN_DBG_PRINT_ALN_SEQ 0x8
#define QUINN_DBG_PRINT_CHAIN 0x10

#define QUINN_SEED_LONG_JOIN (1ULL << 40)
#define QUINN_SEED_IGNORE (1ULL << 41)
#define QUINN_EED_TANDEM (1ULL << 42)
#define QUINN_SEED_SELF (1ULL << 43)

#define QUINN_SEED_SEG_SHIFT 46
#define QUINN_SEED_SEG_MASK (0xffULL << (QUINN_SEED_SEG_SHIFT))

#ifndef kroundup32
#define kroundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))
#endif

#define quinn_seq4_set(s, i, c) ((s)[(i)>>3] |= (uint32_t)(c) << (((i)&7)<<2))
#define quinn_seq4_get(s, i) ((s)[(i)>>3] >> (((i)&7)<<2) & 0xf)
#define MALLOC(type, len) ((type*)malloc((len) * sizeof(type)))
#define CALLOC(type, len) ((type*)calloc((len), sizeof(type)))

#ifdef __cplusplus
extern "C" {
#endif
  typedef struct {
    uint32_t n;
    uint32_t q_pos;
    uint32_t q_span:31, flt:1;
    uint32_t seg_id:31, is_tandem:1;
    const uint64_t *cr;
  } quinn_seed_t;

  typedef struct {
    int n_u, n_a;
    uint64_t *u;
    quinn128_t *a;
  } quinn_seg_t;

  double cputime(void);
  double realtime(void);
  long peakrss(void);
  
  void radix_sort_128x(quinn128_t *beg, quinn128_t *end);
  void radix_sort_64(uint64_t *beg, uint64_t *end);
  uint32_t ks_small_uint32_t(size_t n, uint32_t arr[], size_t kk);

  void quinn_sketch(void *km, const char *str, int len, int w, int k, uint32_t rid, int is_hpc, quinn128_v *p);
  quinn_seed_t *quinn_collect_matches(void *km, int *_n_m, int qlen, int max_occ, int max_max_occ, int dist, const quinn_idx_t *mi, const quinn128_v *mv, int64_t *n_a, int *rep_len, int *n_mini_pos, uint64_t **mini_pos);
  void quinn_seed_mz_flt(void *km, quinn128_v *mv, int32_t q_occ_max, float q_occ_frac);

  double quinn_event_identity(const quinn_reg1_t *r);
  int quinn_write_sam_hdr(const quinn_idx_t *mi, const char *rg, const char *ver, int argc, char *argv[]);
  void quinn_write_paf(kstring_t *s, const quinn_idx_t *mi, const quinn_bseq1_t *t, const quinn_reg1_t *r, void *km, int64_t opt_flag);
  void quinn_write_paf3(kstring_t *s, const quinn_idx_t *mi, const quinn_bseq1_t *t, const quinn_reg1_t *r, void *km, int64_t opt_flag, int rep_len);
  void quinn_write_sam(kstring_t *s, const quinn_bseq1_t *mi, const quinn_reg1_t *t, int n_regs, const quinn_reg1_t *regs);
  void quinn_write_sam2(kstring_t *s, const quinn_idx_t *mi, const quinn_bseq1_t *t, int seg_idx, int n_seg, const int *n_regs, const quinn_reg1_t *const* regs, void *km, int64_t opt_flag);
  void quinn_write_sam3(kstring_t *s, const quinn_idx_t *mi, const quinn_bseq1_t *t, int seg_idx, int reg_idx, int n_seg, const int *n_regss, const quinn_reg1_t *const*, void *km, int64_t opt_flag, int rep_len);

  void quinn_idxopt_init(quinn_idxopt_t *opt);
  const uint64_t *quinn_idx_get(const quinn_idx_t *mi, uint64_t minier, int *n);
  int32_t quinn_idx_cal_max_occ(const quinn_idx_t *mi, float f);
  int quinn_idx_getseq2(const quinn_idx_t *mi,int is_rev, uint32_t rid, uint32_t st, uint32_t en, uint8_t *seq);
  quinn_reg1_t *quinn_align_skeleton(void *km, const quinn_mapopt_t *opt, const quinn_idx_t *mi, int qlen, const char *qstr, int *n_regs_, quinn_reg1_t *regs, quinn128_t *a);
  quinn128_t *quinn_chain_dp(int max_dist_x, int max_dist_y, int bw, int max_skip, int max_iter, int min_cnt, int min_sc, float gap_scale, int is_cdna, int n_segs, int64_t n, quinn128_t *a, int *n_u_, uint64_t **_u, void *km);
  quinn128_t *mg_lchain_dp(int max_dist_x, int max_dist_y, int bw, int max_skip, int max_iter, int min_cnt, int min_sc, float chn_pen_gap, float chn_pen_skip, int is_cdna, int n_segs, int64_t n, quinn128_t *a, int *n_u_, uint64_t **_u, void *km);
  quinn128_t *mg_lchain_rmq(int max_dist, int max_dist_inner, int bw, int max_chn_skip, int cap_rmq_size, int min_cnt, int min_sc, float chn_pen_gap, float chn_pen_skip, int64_t n, quinn128_t *a, int *n_u_, uint64_t **_u, void *km);

  void quinn_mark_alt(const quinn_idx_t *mi, int n, quinn_reg1_t *r);
  void quinn_split_reg(quinn_reg1_t *r, quinn_reg1_t *r2, int n, int qlen, quinn128_t *a, int is_qstrand);
  void quinn_sync_regs(void *km, int n_regs, quinn_reg1_t *regs);
  int quinn_squeeze_a(void *km, int n_regs, quinn_reg1_t *regs, quinn128_t *a);
  int quinn_set_sam_pri(int n, quinn_reg1_t *r);
  void quinn_set_parent(void *km, float mask_level, int mask_len, int n, quinn_reg1_t *r, int sub_diff, int hard_mask_level, float alt_diff_frac);
  void quinn_select_sub(void *km, float pri_ratio, int min_diff, int best_n, int check_strand, int min_stand_sc, int *n_, quinn_reg1_t *r);
  void quinn_select_sub_multi(void *km, float pri_ratio, float pri1, float pri2, int max_gap_ref, int min_diff, int best_n, int n_segs, const int *qlens, int *n_, quinn_reg1_t *r);
  int quinn_filter_strand_retained(int n_reg, quinn_reg1_t *r);
  void quinn_filter_regs(const quinn_mapopt_t *opt, int qlen, int *n_regs, quinn_reg1_t *regs);
  void quinn_hit_sort(void *km, int *n_regs, quinn_reg1_t *r, float alt_diff_frac);
  void quinn_set_mapq(void *km, int n_regs, quinn_reg1_t *regs, int min_chain_sc, int match_sc, int rep_len, int is_sr);
  void quinn_update_dp_max(int qlen, int n_regs, quinn_reg1_t *regs, float frac, int a, int b);

  void quinn_est_err(const quinn_idx_t *mi, int qlen, int n_regs, quinn_reg1_t *args, const quinn128_t *a, int32_t n, const uint64_t *mini_pos);

  quinn_seg_t *quinn_seg_gen(void *km, uint32_t hash, int n_segs, const int *qlens, int n_regs0, const quinn_reg1_t *regs0, int *n_regs, quinn_reg1_t **regs, const quinn128_t *a);
  void quinn_seg_free(void *km, int n_segs, quinn_seg_t *segs);
  void quinn_pair(void *km, int max_gap_ref, int dp_bonus, int sub_diff, int match_sc, const int *qlens, int *n_regs, quinn_reg1_t **regs);

  FILE *quinn_split_init(const char *prefix, const quinn_idx_t *mi);
  quinn_idx_t *quinn_split_merge_prep(const char *prefix, int n_splits, FILE **fp, uint32_t *n_seq_part);
  int quinn_split_merge(int n_segs, const char **fn, const quinn_mapopt_t*, int n_plit_idx);
  void quinn_split_rm_tmp(const char *prefix, int n_splits);

  void quinn_err_puts(const char *str);
  void quinn_err_fwrite(const void *p, size_t size, size_t nitems, FILE *fp);
  void quinn_err_fread(void *p, size_t size, size_t nitems, FILE *fp);

  static inline float mg_log2(float x) {
    union {
      float f;
      uint32_t i;
    } 
    z = {x};
    float log_2 = ((z.i >> 23) & 255) - 128;
    z.i &= ~(255 << 23);
    z.i += 127 << 23;
    log_2 += (-0.34484843f * z.f + 2.02466578f) * z.f - 0.67487759f;
    return log_2;
  }
#ifdef __cplusplus
}
#endif

#endif
