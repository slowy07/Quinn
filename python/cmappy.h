#ifndef CMAPPY_H
#define CMAPPY_H

#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "quinn.h"
#include "kseq.h"
KSEQ_DECLARE(gzFile)

typedef struct {
  const char *ctg;
  int32_t ctg_start, ctg_end;
  int32_t qry_start, qry_end;
  int32_t blen, mlen, MM, ctg_len;
  uint8_t mapq, is_primary;
  int8_t strand, trans_strand;
  int32_t seg_id;
  int32_t n_cigar32;
  uint32_t *cigar32;
} quinn_hitpy_t;

static inline void quinn_reg2hitpy(const quinn_idx_t *mi, quinn_reg1_t *r, quinn_hitpy_t *h) {
  h->ctg = mi->seq[r->rid].name;
	h->ctg_len = mi->seq[r->rid].len;
	h->ctg_start = r->rs, h->ctg_end = r->re;
	h->qry_start = r->qs, h->qry_end = r->qe;
	h->strand = r->rev? -1 : 1;
	h->mapq = r->mapq;
	h->mlen = r->mlen;
	h->blen = r->blen;
	h->NM = r->blen - r->mlen + r->p->n_ambi;
	h->trans_strand = r->p->trans_strand == 1? 1 : r->p->trans_strand == 2? -1 : 0;
	h->is_primary = (r->id == r->parent);
	h->seg_id = r->seg_id;
	h->n_cigar32 = r->p->n_cigar;
	h->cigar32 = r->p->cigar;
}

static inline void quinn_free_reg1(quinn_reg1_t *r) {
  free(r -> p);
}

static inline kseq_t *quinn_fastx_open(const char *fn) {
  gzFile fp;
  fp = fn && strcmp(fn, "-") != 0 ? gzopen(fn, "r") :gzopen(fileno(stdin),"r");
  return kseq_init(fp);
}

static inline void quinn_fastx_close(kseq_t *ks) {
  gzFile fp;
  fp = ks -> f -> f;
  kseq_destroy(ks);
  gzclose(fp);
}

#endif
