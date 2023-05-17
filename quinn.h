#ifndef QUINN_H
#define QUINN_H

#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#define QUINN_VERSION "1.0.0"

#define QUINN_F_NO_DIAG (0x001LL)
#define QUINN_F_NO_DUAL (0x002LL)
#define QUINN_F_CIGAR (0x004LL)
#define QUINN_F_OUT_SAM (0x008LL)
#define QUINN_F_NO_QUAL (0x010LL)
#define QUINN_F_OUT_CG (0x020LL)
#define QUINN_F_OUT_CS (0x040LL)
#define QUINN_F_SPLICE (0x080LL)
#define QUINN_F_SPLICE_FOR (0x100LL)
#define QUINN_F_SPLTCE_REV (0x200LL)
#define QUINN_F_NO_JOIN (0x400LL)
#define QUINN_F_OUT_CS_LONG (0x800LL)
#define QUINN_F_SR (0x1000LL)
#define QUINN_F_FRAG_MODE (0x2000LL)
#define QUINN_F_NO_PRINT_2ND (0x4000LL)
#define QUINN_F_2_IO_THREADS (0x8000LL)
#define QUINN_F_LONG_CIGAR (0x10000LL)
#define QUINN_F_INDEPEND_SEG (0x20000LL)
#define QUINN_F_SPLICE_FLANK (0x40000LL)
#define QUINN_F_SOFTCLIP (0x80000LL)
#define QUINN_F__FOR_ONLY (0x100000LL)
#define QUINN_F_REV_ONLY (0x200000LL)
#define QUINN_F_HEAP_SORT (0x400000LL)
#define QUINN_F_ALL_CHAINS (0x800000LL)
#define QUINN_F_OUT_MD (0x1000000LL)
#define QUINN_F_COPY_COMMENT (0x2000000LL)
#define QUINN_F_EQX (0x4000000LL)
#define QUINN_F_PAF_NO_HIT (0x8000000LL)
#define QUINN_F_NO_END_FLT (0x10000000LL)
#define QUINN_F_HARD_MLEVEL (0x20000000LL)
#define QUINN_F_SAM_HIT_ONLY (0x40000000LL)
#define QUINN_F_RMQ (0x80000000LL)
#define QUINN_F_QSTRAND (0x100000000LL)
#define QUINN_F_NO_INV (0x200000000LL)
#define QUINN_F_NO_HASH_NAME (0x400000000LL)
#define QUINN_F_SPLICE_OLD (0x800000000LL)
#define QUINN_F_SECONDARY_SEQ (0x1000000000LL)

#define QUINN_I_HPC 0x1
#define QUINN_I_NO_SEQ 0x2
#define QUINN_I_NO_NAME 0x4

#define QUINN_IDX_MAGIC "MMI\2"

#define QUINN_MAX_SEG 255

#define QUINN_CIGAR_MATCH 0
#define QUINN_CIGAR_INS 1
#define QUINN_CIGAR_DEL 2
#define QUINN_CIGAR_N_SKIP 3
#define QUINN_CIGAR_SOFTCLIP 4
#define QUINN_CIGAR_HARDCLIP 5
#define QUINN_CIGAR_PADDING 6
#define QUINN_CIGAR_EQ_MATCH 7
#define QUINN_CIGAR_X_MISMATCH 8

#define QUINN_CIGAR_STR "MIDNSHP=X8"

#ifdef __cplusplus
extern "C" {
#endif
  // simulate 128-bit integers and array
  typedef struct {uint64_t x, y;} quinn128_t;
  typedef struct {size_t n, m; quinn128_t *a;} quinn128_v;

  // Quinn index
  typedef struct {
    // name of database sequence
    char *name;
    // offset in quinn_idx_t::S
    uint64_t offset;
    // length
    uint32_t len;
    uint32_t is_alt;
  } quinn_idx_seq_t;

  typedef struct {
    int32_t b, w, t, k, flag;
    // number of reference sequence
    uint32_t n_seq;
    int32_t index;
    int32_t n_alt;
    quinn_idx_seq_t *seq;
    uint32_t *S;
    struct quinn_idx_bucket_s *B;
    struct quinn_idx_intv_s *I;
    void *km, *h;
  } quinn_idx_t;

  typedef struct {
    // the capacity of cigar[]
    uint32_t capacity;
    // DP score;
    // score of the max-scoring segment;
    // score of the best alternate mappings
    int32_t dp_source, dp_max, dp_max2;
    // number ofambiguous bases;
    // transcript strand;
    // 0 for unkbown, i for +,  for -
    uint32_t n_ambi:30, transstrand:2;
    // number of cigar operations in cigar[]
    uint32_t cigar[];
  } quinn_extra_t;

  typedef struct {
    // ID for internal uses
    int32_t id;
    // number of minimizers; if on the reverse strand
    int32_t cnt;
    // reference index;
    // if this is an aligment from inversion rescue
    int32_t rid;
    // DP alignment score
    int32_t score;
    // query start and end;
    // reference start and end
    int32_t qs, qe, rs, re;
    // parent == id if primary;
    // best alternate mapping score
    int32_t parent, subsc;
    // offet in the a[] array
    int32_t as;
    // seeded exact match length;
    // seeded alignment block length
    int32_t mlen, blen;
    // number of of suboptimal mappings
    int32_t n_sub;
    // initial chaining sscore
    int32_t score0;
    uint32_t mapq:8, split:2, rev:1, inv:1, sam_pri:1, proper_frag:1, pe_thru:1, seg_split:1, seg_id:8, split_inv:1, is_alt:1, strand_retained:1, dummy:5;
    uint32_t hash;
    float div;
    quinn_extra_t *p;
  } quinn_reg1_t;

  // indexing and mappings options
  typedef struct  {
    short k, w, flag, bucket_bits;
    int64_t mini_batch_size;
    uint64_t batch_size;
  } quinn_idxopt_t;

  typedef struct {
    int64_t flag;
    int seed;
    // score threshold for SDUST
    int sdust_thres;
    // max quary length
    int max_qlen;
    // bandwith
    int bw, bw_long;
    // break a chain if there are no minimizers
    // in a ``max_gap`` windwow
    int max_gap, max_gap_ref;
    int max_frag_len;
    int max_chain_skip, max_chain_iter;
    // min number of minimzers on each chain
    int min_cnt;
    // min chaining score
    int main_chain_score;
    float chain_gap_scale;
    float chain_skip_scale;
    int rmq_size_cap, rmq_inner_dist;
    int rmq_rescue_ratio;
    float mask_level;
    int mask_len;
    float pri_ratio;
    int best_n;
    float alt_drop;
    // matching score, mismatch, gap-open and gap-ext penalties
    int a, b, q, e , q2, e2;
    int sc_ambi;
    // cost of non-canonical splicing sites
    int noncan;
    int junc_bonus;
    // break an if alignment score drops to fast along the diagonal
    int zdrop, zdrop_inv;
    int end_bonus;
    // drop an alignment if the score of the max scoring segment is below this threshold
    int min_dp_max;
    int min_ksw_len;
    int anchor_ext_len, anchore_ext_shift;
    // drop an alignment if BOTH ends are clipped aboce this ratio
    float max_clip_ratio;
    int rank_min_len;
    float rank_frac;
    int pe_ori, pe_bonus;
    // only used by quinn_mapopt_update*();
    float mid_occ_frac;
    float q_occ_frac;
    int32_t min_mid_occ, max_mid_occ;
    int32_t mid_occ;
    int32_t max_occ, max_max_occ, occ_dist;
    // size of a batch of  query bases to processs in parallel
    int64_t mini_batch_size;
    int64_t max_sw_mat;
    int64_t cap_kalloc;
    const char *split_prefix;
  } quinn_mapopt_t;

  typedef struct {
    int is_idx, n_parts;
    int64_t idx_size;
    quinn_idxopt_t opt;
    FILE *fp_out;
    union {
      struct quinn_bseq_file_s *seq;
      FILE *idx;
    } fp;
  } quinn_idx_reader_t;

  // memory buffer for thread-local storeage during mapping
  struct quinn_tbuf_s {
    void *km;
    int rep_len, fraq_gap;
  };

  typedef struct quinn_tbuf_s quinn_tbuf_t;

  // global variables
  
  // verbose level: 0 for no info, 1 for error,  2 for warning, 3 for message
  extern int quinn_verbose, quinn_dbg_flag;
  extern double quinn_realtime0;

  /**
   * default paramaters
   * @param preset
   *        NULL to set all paramateres as default
   *        otherwise apply preset to affected parameters
   * @param io
   *        pointer to indexing parametes
   * @param mo
   *        pointer to mapping parameters
   * @return 0 if success, -1 if __preset__ unkbown
   */
  int quinn_set_opt(const char *preset, quinn_idxopt_t *io, quinn_mapopt_t *mo);
  int quinn_check_opt(const quinn_idxopt_t *io, const quinn_mapopt_t *mo);

  /**
   * update quinn_mapopt_t::mid_occ via quinn_mapopt_t::mid_occ_fraq
   * if quinn_mapopt_t::mid_occ 0, this function sets it to a number such that
   * no more than quinn_mapopt_t::mid_occ_frac of minimizers in the index have a
   * higher occurence
   */
  void quinn_mapopt_update(quinn_mapopt_t *opt, const quinn_idx_t *mi);
  void quinn_maptopt_max_intron_len(quinn_mapopt_t *opt, int max_intron_len);

  /**
   * initialize an index reader
   * @param fn
   *        index or fasta/fastq filename
   * @param opt
   *        indexing parameters
   * @param fn_out
   *        if not NULL, write build index to this file
   */
  quinn_idx_reader_t *quinn_idx_reader_open(const char *fn, const quinn_idxopt_t *opt, const char *fn_out);

  /**
   * destroy/deallocate an index reader
   * @param r
   *        index reader
   */
  void quinn_idx_reader_close(quinn_idx_reader_t *r);
  int quinn_idx_reader_eof(const quinn_idx_reader_t *r);

  /**
   * check wheter the file contains a quinn index
   * @param fn filename
   */
  int64_t quinn_idx_is_idx(const char *fn);

  /*
   * load a part of an index
   * given a uni-part index, this function loads the entire index to memory.
   * given a multi-part index, it loads one part onyl and places the file pointer
   * at the end of that part.
   */
  quinn_idx_t *quinn_idx_load(FILE *fp);

  /*
   * create an index from string in memory
   * @param w minimzers window size
   * @param k minimzers k-mer size
   * @param is_hpc use HPC k-mer if true
   * @param bucket_bits number of bits for the first level of the hash table
   * @param n number of sequences
   * @param seq sequences in A/C/G/T
   * @param name sequence name
   */
  quinn_idx_t *quinn_idx_str(int w, int k, int is_hpc, int bucket_bits, int n, const char **seq, const char **name);

  /**
   * print index statistic to stderr
   * @param quinn quinn index
   */
  void quinn_idx_start(const quinn_idx_t *idx);

  /**
   * destroy/deallocate an index
   * @param r quinn index
   */
  void quinn_idx_destroy(quinn_idx_t *quinn);

  /*
   * initialize a thread local buffer for mapping
   * each mapping thread requires a buffer specific to the thread.
   * the primary purpose of this buffer is to reduce frequent heap
   * allocations accross threads. a buffer shall not be ued by two or more
   * threads.
   */
  quinn_tbuf_t *quinn_tbuf_init(void);

  /**
   * destroy/deallocate a thread-local buffer for mapping
   * @param b the buffer
   */
  void quinn_tbuf_destroy(quinn_tbuf_t *b);
  void quinn_tbuf_get_km(quinn_tbuf_t *b);

  /**
   * align a query sequence againts an index
   * this function possible finds multiple alignments of the query sequence.
   * the returned array and quinn_reg1_t::p field of each element are allocated
   * with malloc()
   * 
   * @param q1 quinn index
   * @param l_seq length of the query sequence
   * @param n_regs the query sequence
   * @param b thread local buffer
   * @param opt mapping parameters
   * @param name query name, used for overlaping and debugging
   */
  quinn_reg1_t *quinn_map(const quinn_idx_t *qi, int l_seq, const char *seq, int *n_regs, quinn_tbuf_t *b, const quinn_mapopt_t *opt, const char *name);
  void quin_map_frag(const quinn_idx_t *qi, int n_segs, const int *qlens, const char **seqs, int *n_regs, quinn_reg1_t **regs, quinn_tbuf_t *b, const quinn_mapopt_t *opt, const char *qname);

  /**
   * initialize an index reader
   * @param fn index or fasta/fastq filename
   * @param opt indexing parameters
   * @param fn_out if not NULL, write built index to this file
   */
  quinn_idx_reader_t *quinn_idx_reader_open(const char *fn, const quinn_idxopt_t *opt, const char *fn_out);

  /**
   * read/build an index
   * if the input file is an index file, this function reads one part of the index and returns.
   * if the input file is a sequence file (fasta or fastq). this function constructs the index for about
   * ``quin_idxopt_t::batch_size`` bases. importantly, for a huge collections of sequences, this function may
   * only return an indexx for apart of sequencez. it neds to be repeatedly called to traverse the entire sequence file
   * @param r index reader
   * @param n_threads number of threads for constructing index
   */
  quinn_idx_t *quinn_idx_reader_read(quinn_idx_reader_t *r, int n_threads);

  /**
   * destroy/deallocate an index reader
   * @param r index reader
   */
  void quinn_idx_reader_close(quinn_idx_reader_t *r);
  int quinn_idx_reader_eof(const quinn_idx_reader_t *a);

  /**
   * check wheter the file contains a quinn index
   * @param fn filename
   */
  int64_t quinn_idx_is_idx(const char *fn);

  /**
   * load a part of an index
   * given a uni-part index, this function loads the entire index into memory
   * given a multi-part index, it loads one part only and places the file pointer
   * at the end of that part.
   * @param fp pointer to FILE object
   */
  quinn_idx_t *quinn_idx_load(FILE *fp);

  /**
   * append an index to file
   * @param fp pointer to FILE object
   * @param qi quinn index
   */
  void quinn_idx_dump(FILE *fp, const quinn_idx_t *qi);

  /**
   * create an index from strings in memory
   * @param w minimzer window size
   * @param k k-mer size
   * @param is_hpc use HPC k-mer if true
   * @param n of bits for the first level of the hash table
   */
  quinn_idx_t *quinn_idx_str(int w, int k, int is_hpc, int bucket_bits, int n, const char **seq, const char **name);

  /**
   * print index statistic to stderr
   * @param qi quinn index
   */
  void quinn_idx_stat(const quinn_idx_t *idx);

  /**
   * destroy/deallocate an index
   * @param r quinn index
   */
  void quinn_idx_destroy(quinn_idx_t *quinn);
}




#endif

