#ifndef __CACHE_H__
#define __CACHE_H__

#include <inttypes.h>
#include "repl_hawkeye.h"

#define MAIN_MEMORY_LATENCY	200

//#define KMEANS
#define DIST // Reuse Distance HashTable
//#define HITC // Hitcount HashTable
//#define FIFO
//#define LRU
//#define HAWKEYE

#define HASH_KEY(key) (key >> 6) % 512

typedef struct tagstore
{
	bool			valid;	//* valid bit (T: valid, F: invalid)
	bool			dirty;	//* dirty bit (T: dirty, F: clean)
	uint64_t	tag;		//* cache tag
	uint32_t	repl;		//* used for replacement policy
	bool			used;
	uint64_t	pc;	
} tagstore_t;

typedef enum cache_type {DIRECT_MAPPED, SET_ASSOCIATE, FULLY_ASSOCIATE} cache_type_t;

typedef struct cache cache_t;

typedef struct reuse_mon	reuse_t;

typedef struct Node
{
    uint64_t id;
    uint64_t hitc;
	uint64_t distance;
	uint64_t mark;
    Node* hashNext;
} Node;

struct reuse_mon
{
	uint64_t	reuse_hit;
	uint64_t	data_change;
	uint64_t	total_acc;
};

struct cache
{
	uint64_t	size;
	uint32_t	way;
	uint32_t	set;
	uint32_t	line;
	uint32_t	latency;

	cache_type_t	type;
	tagstore_t**	tag;

	reuse_t**	reuse;

	uint64_t	index_bit;
	uint64_t	offset_bit;

	uint64_t	hit_cnt;
	uint64_t	miss_cnt;
	uint64_t	total_access_cnt;

	bool			is_first;
	bool			is_last;
	cache_t*	_next_cache;	//* a pointer to a lower level's cache

	uint32_t	**rrpv;
	uint32_t	SAMPLER_WAY;
	uint32_t	SAMPLER_SET;
	uint32_t							H_SAMPLER_CACHE_SIZE;
	uint32_t							H_SHCT_SIZE_BITS;
	uint32_t							H_SHCT_SIZE;
	uint64_t*							perset_mytimer;
	HAWKEYE_PC_PREDICTOR*	demand_predictor;
	HAWKEYE_PC_PREDICTOR* prefetch_predictor;
	uint64_t**						signatures;
	bool**								prefetched;
	OPTgen*								perset_optgen;
	std::vector<std::map<uint64_t, ADDR_INFO>> *addr_history;	//* Sampler
	uint64_t    distance_sum;
	uint64_t	hitcount_sum;
	uint64_t	table_data_num;
};

extern Node* hashTable[512];

extern uint64_t executed_instrs;

uint32_t  findHashData(cache_t* _cache, uint64_t key);
void  AddHashData(cache_t* _cache, uint64_t key, Node* node);
int WriteFile(std::string fname, Node* htable[]);

cache_t*	init_cache_structure(uint64_t size, uint32_t way, uint32_t set, uint32_t line, uint32_t latency, cache_type_t type, bool is_first, bool is_last);
bool			cache_access(cache_t* _cache, uint64_t _addr, uint64_t _pc, bool _wb);
uint32_t	do_cache_access(cache_t* _cache, uint64_t _addr, uint64_t _pc, bool _wb);
void			connect_cache(cache_t* _master, cache_t* _slave);
int				is_way_full(cache_t* _cache, uint64_t _addr);
uint32_t	find_victim_way(cache_t* _cache, uint64_t _addr);
uint32_t	cache_evict_line(cache_t* _cache, uint64_t _addr);
void			cache_insert_line(cache_t* _cache, uint64_t _addr, uint32_t _way, uint64_t _pc);
void			cache_FIFO_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit);
void			cache_LRU_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit);
void 			cache_HITC_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, uint64_t _pc);
void 			cache_DIST_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, uint64_t _pc);
void			cache_HawkEye_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, bool _wb, uint64_t _pc);
void			init_cache_replacement(cache_t* _cache);
void 			cache_KMEANS_replacement (cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, uint64_t _pc);

uint64_t	u64_log2(uint64_t n);

void HawkEye_replace_addr_history_element(cache_t* _cache, unsigned int sampler_set);
void HawkEye_update_addr_history_lru(cache_t* _cache, unsigned int sampler_set, unsigned int curr_lru);
bool SAMPLED_SET(uint64_t set, uint32_t num_cores, uint32_t LLC_SET);


#endif
