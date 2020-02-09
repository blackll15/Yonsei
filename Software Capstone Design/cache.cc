#include "cache.h"
#include "repl_hawkeye.h"
#include <malloc.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

#define K_COUNT 2

int DATA_COUNT = 0;
int k0_DATA_COUNT = 0;
int k1_DATA_COUNT = 0;
float k0_x;
float k0_y;
float k1_x;
float k1_y;

    /*
    { 
    pos* k = new pos[K_COUNT];
    pos* center = new pos[K_COUNT];
    double count_Group[K_COUNT]={0,};
    vector< pos > datas;
    vector< double > distance[K_COUNT];
    //data  --> 여기 부분을 random생성이 아니라 저희 데이터를 넣기
    pos tmp;
	for (int i = 0; i < 512; i++){
		if(findHashDataTraverse(_cache, i) != NULL && findHashDistanceTraverse(_cache, i) != 0){		
        	tmp.x = findHashDataTraverse(_cache, i);
        	tmp.y = findHashDistanceTraverse(_cache, i);
			datas.push_back(tmp);
			DATA_COUNT++;
    	}
	}
    //cout<<"("<<tmp.x<<","<<tmp.y<<")"<<endl; //데이터 출력
    //random k, init
    for(int i=0; i<K_COUNT; i++)
    {
        k[i] = datas[i];
        center[i].x = datas[i].x;
        center[i].y = datas[i].y;
        distance[i].resize( DATA_COUNT );
    }
    bool loop = true;
    while(loop){ //when the k-positions are all same with next position.
    //center init
    for(int i=0;i<K_COUNT; i++){
        center[i].x = 0;
        center[i].y = 0;
        count_Group[i] = 0;
    }
    // distance
    for(int i=0;i<datas.size(); i++){
        for(int j=0;j<K_COUNT; j++){
            double tmp_distance = sqrt (pow( k[j].x - datas[i].x, 2) + pow( k[j].y - datas[i].y, 2) );
            distance[j][i] = tmp_distance;
        }
    }
    //get center
    for(int i=0;i<datas.size(); i++){
        double min = distance[0][i];
        int min_j = 0;
	for(int j=1;j<K_COUNT; j++){
            if(min > distance[j][i]){
                min = distance[j][i];
                min_j = j;
            }
        }
        center[ min_j ].x += datas[i].x;
        center[ min_j ].y += datas[i].y;
        count_Group[min_j]++;
    }
    //change K
    int same_count = 0;
    for(int i=0;i<K_COUNT; i++){
        if(count_Group[i] != 0){
            if((center[i].x / count_Group[i]) == k[i].x  && (center[i].y / count_Group[i]  == k[i].y)) same_count ++;
                k[i].x = center[i].x / count_Group[i];
                k[i].y = center[i].y / count_Group[i];
            }
            if(same_count == K_COUNT){
                loop=false;
            }
            //cout<<fixed<<setprecision(2);
            //cout<<"("<<k[i].x<<","<<k[i].y<<") ";
        } //cout<<endl;
    }//end of loop

	int cache_friendly = -1;
	int cache_unfriendly = -1;
	if(k[0].x > k[1].x){ cache_friendly = 0; cache_unfriendly = 1; }
	else{ cache_friendly = 1; cache_unfriendly = 0;}
    
    uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	bool has_max = false;
	if(!_hit)_cache->tag[_way][req_idx].repl = _cache->way - 2;
	else if(_cache->tag[_way][req_idx].repl > 0)_cache->tag[_way][req_idx].repl--;
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;	
	}
	for(int count = 0; !has_max; count++)
	{	
		for (uint32_t w=0; w<_cache->way; w++)
		{
			uint32_t x = findHashData(_cache, _cache->tag[w][req_idx].pc);
			uint32_t y = findHashDistance(_cache, _cache->tag[w][req_idx].pc);
			double dist_zero = 0;
			double dist_one = 0;
			int clas = -1;
			if(x != NULL && y != NULL){
            	dist_zero = sqrt (pow( k[0].x - x, 2) + pow( k[0].y - y, 2) );
				dist_one = sqrt (pow( k[1].x - x, 2) + pow( k[1].y - y, 2) );
				if (dist_zero > dist_one)clas = 1;
				else clas = 0;

				if(count > _cache->way || clas == cache_unfriendly)
				{
					_cache->tag[w][req_idx].repl++;
				}
			}
			else{
				_cache->tag[w][req_idx].repl++;
			}

		}
		for (uint32_t w=0; w<_cache->way; w++)
		{
			if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;
		}

	} 
}
*/
 
class pos{
public:
        double x;
        double y;
};

uint64_t u64_log2(uint64_t n)
{
	uint64_t power;
	for (power=0; n>>=1; ++power);
	return power;
}

cache_t*	init_cache_structure(uint64_t size, uint32_t way, uint32_t set, uint32_t line, uint32_t latency, cache_type_t type, bool is_first, bool is_last)
{
	cache_t* new_cache = (cache_t*)malloc(sizeof(cache_t));

	new_cache->size = size;
	new_cache->way = way;
	new_cache->set = set;
	new_cache->line = line;
	new_cache->latency = latency;
	new_cache->type = type;
	new_cache->is_first = is_first;
	new_cache->is_last = is_last;

	new_cache->hitcount_sum = 0;
	new_cache->distance_sum = 0;
	new_cache->table_data_num = 1;

	switch (type)
	{
		case DIRECT_MAPPED:
		{
			new_cache->set = size / line;
			new_cache->way = 1;
			break;
		}
		case SET_ASSOCIATE:
		{
			new_cache->set = size / way / line;
			break;
		}
		case FULLY_ASSOCIATE:
		{
			new_cache->way = size / line;
			new_cache->set = 1;
			break;
		}
		default:
			break;
	}

	new_cache->index_bit = u64_log2((uint64_t)(new_cache->set));
	new_cache->offset_bit = u64_log2((uint64_t)(new_cache->line));

	new_cache->tag = (tagstore_t**)malloc(sizeof(tagstore_t*) * new_cache->way);
	for (uint32_t w=0; w<new_cache->way; w++)
	{
		new_cache->tag[w] = (tagstore_t*)malloc(sizeof(tagstore_t) * new_cache->set);
		for (uint32_t s=0; s<new_cache->set; s++)
		{
			new_cache->tag[w][s] = {false, false, 0, w, false};	//* {valid, dirty, tag, repl, used}
		}
	}

	new_cache->reuse = (reuse_t**)malloc(sizeof(reuse_t*) * new_cache->way);
	for (uint32_t i=0; i<new_cache->way; i++)
	{
		new_cache->reuse[i] = (reuse_t*)malloc(sizeof(reuse_t) * new_cache->set);
		for (uint32_t j=0; j<new_cache->set; j++)
		{
			//* reuse_hit / data_change
			new_cache->reuse[i][j].reuse_hit = 0;
			new_cache->reuse[i][j].total_acc = 0;
			new_cache->reuse[i][j].data_change = 0;
		}
	}

	new_cache->total_access_cnt = 0;
	new_cache->hit_cnt = 0;
	new_cache->miss_cnt = 0;

	new_cache->_next_cache = nullptr;

	init_cache_replacement(new_cache);

	return new_cache;
}

void connect_cache(cache_t* _master, cache_t* _slave)
{
	_master->_next_cache = _slave;
}

//* single-layer cache access
bool cache_access(cache_t* _cache, uint64_t _addr, uint64_t _pc, bool _wb)
{
	uint64_t req_tag = (_addr >> (uint64_t)(_cache->index_bit + _cache->offset_bit));
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	_cache->total_access_cnt++;
	for (uint64_t w=0; w<_cache->way; w++)
	{
		if ((_cache->tag[w][req_idx].tag == req_tag) && (_cache->tag[w][req_idx].valid))
		{
			_cache->tag[w][req_idx].used = true;
			_cache->reuse[w][req_idx].total_acc++;
			#ifdef LRU
			cache_LRU_replacement(_cache, _addr, w, true);
			#endif
			#ifdef HITC
			cache_HITC_replacement(_cache, _addr, w, true, _pc);
			#endif
			#ifdef DIST
			cache_DIST_replacement(_cache, _addr, w, true, _pc);
			#endif
			#ifdef HAWKEYE
			cache_HawkEye_replacement(_cache, _addr, w, true, false, _pc);
			#endif
			#ifdef KMEANS
			if (DATA_COUNT > 10) {	cache_KMEANS_replacement(_cache, _addr, w, true, _pc); }
			else { cache_DIST_replacement(_cache, _addr, w, true, _pc); }
			#endif
			_cache->hit_cnt++;
			return true;
		}
	}
	_cache->miss_cnt++;
	return false;
}

uint32_t  findHashData(cache_t* _cache, uint64_t key)
{
    uint64_t hash_key = HASH_KEY(key);
	for(Node* curnode = hashTable[hash_key]; curnode != NULL; curnode = curnode->hashNext)
	{
		if(curnode->id == key)
		{
			return curnode->hitc;
		}
	}
	return NULL;
}
uint32_t  findHashDataTraverse(cache_t* _cache, uint64_t key)
{
    uint64_t hash_key = HASH_KEY(key);
    for(Node* curnode = hashTable[hash_key]; curnode != NULL; curnode = curnode->hashNext)
    {
        return curnode->hitc;
    }
    return NULL;
}
uint32_t  findHashDistanceTraverse(cache_t* _cache, uint64_t key)
{
    uint64_t hash_key = HASH_KEY(key);
    for(Node* curnode = hashTable[hash_key]; curnode != NULL; curnode = curnode->hashNext)
    {
        return curnode->distance;
    }
    return NULL;
}

uint32_t  findHashDistance(cache_t* _cache, uint64_t key)
{
    uint64_t hash_key = HASH_KEY(key);
	for(Node* curnode = hashTable[hash_key]; curnode != NULL; curnode = curnode->hashNext)
	{
		if(curnode->id == key)
		{
			return curnode->distance;
		}
	}
	return NULL;
}
void  AddHashData(cache_t* _cache, uint64_t key, Node* node)
{
    uint64_t hash_key = HASH_KEY(key);
    if (hashTable[hash_key] == NULL)
    {
        hashTable[hash_key] = node;
	_cache->table_data_num++;
	DATA_COUNT++;
    }
    else
    {
		int depth = 0;
		for(Node* curnode = hashTable[hash_key]; curnode != NULL; curnode = curnode->hashNext)
		{
			depth++;
			if(curnode->id == node->id)
			{
				curnode->hitc++;
				curnode->distance = node->mark - curnode->mark;
				_cache->hitcount_sum++;
				_cache->distance_sum++;
				if(k0_DATA_COUNT == 0)
				{
					k0_DATA_COUNT++;
					k0_x += curnode->hitc;
					k0_y += curnode->distance;
				}
				else if(k1_DATA_COUNT == 0)
				{
					k1_DATA_COUNT++;
					k1_x += curnode->hitc;
					k1_y += curnode->distance;
				}
				else
				{
					if( pow(k0_x / k0_DATA_COUNT - curnode->hitc, 2) + pow(k0_y / k0_DATA_COUNT - curnode->distance, 2)
							> pow(k1_x / k1_DATA_COUNT - curnode->hitc, 2) + pow(k1_y / k1_DATA_COUNT - curnode->distance, 2))
					{
						k1_DATA_COUNT++;
						k1_x += curnode->hitc;
						k1_y += curnode->distance;
					}
					else
					{
						k0_DATA_COUNT++;
						k0_x += curnode->hitc;
						k0_y += curnode->distance;
					}
				}
				return;	
			
			}
			if(depth == 2)
			{
			_cache->hitcount_sum -= curnode->hitc;
			_cache->distance_sum -= curnode->distance;
			_cache->table_data_num--;
			DATA_COUNT--;
			hashTable[hash_key]->hashNext == NULL;// save at most 2 element in FIFO 
			}
		}
    		node->hashNext = hashTable[hash_key];
		hashTable[hash_key] = node;
    		_cache->table_data_num++;
    		DATA_COUNT++;
    }	
}


//* multi-layer cache access
uint32_t do_cache_access(cache_t* _cache, uint64_t _addr, uint64_t _pc, bool _wb)
{
	uint32_t total_latency = 0;
	uint32_t evict_way;
	
	if (cache_access(_cache, _addr, _pc, _wb))
	{
		if(_cache->is_last){
			Node* node = (Node*)malloc(sizeof(Node));
        		node->id = _pc;
			node->hitc = 0;
			node->mark = executed_instrs;
			node->distance = node->mark - executed_instrs;
        		node->hashNext = NULL;
			AddHashData(_cache, _pc, node);
		}
		return _cache->latency;
	}
	else
	{
		if (_cache->is_last)
		{
			//* LLC miss - main memory access occurs:
			total_latency += MAIN_MEMORY_LATENCY;
			if (is_way_full(_cache, _addr) == -1)
				evict_way = cache_evict_line(_cache, _addr);
			cache_insert_line(_cache, _addr, is_way_full(_cache, _addr), _pc);
		}
		else
		{
			uint64_t evict_idx, evict_addr;
			bool dirty;
			total_latency += do_cache_access(_cache->_next_cache, _addr, _pc, false) + _cache->latency;
			if (is_way_full(_cache, _addr) == -1)
			{
				evict_way = cache_evict_line(_cache, _addr);
	
				evict_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
				evict_addr = (_cache->tag[evict_way][evict_idx].tag << (_cache->offset_bit + _cache->index_bit)) + (evict_idx << _cache->offset_bit);
				dirty = _cache->tag[evict_way][evict_idx].dirty;
			}
			cache_insert_line(_cache, _addr, is_way_full(_cache, _addr), _pc);

			if (dirty)
				total_latency += do_cache_access(_cache->_next_cache, evict_addr, _pc, true);			
		}
	}

	return total_latency;
}

int is_way_full(cache_t* _cache, uint64_t _addr)	//* -1: all sets in the way are full, w>=0: way(w) is avail.
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	for (uint32_t w=0; w<_cache->way; w++)
		if (!(_cache->tag[w][req_idx].valid))
			return w;
	return -1;
}

//* find_victim_way(): find empty way or a way should be evicted for a new cache line.
uint32_t find_victim_way(cache_t* _cache, uint64_t _addr)
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	uint32_t victim_way = UINT32_MAX;
	
	//* for FIFO
	#ifdef FIFO
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->tag[w][req_idx].repl == 0)
		{
			victim_way = w;
			break;
		}
	}
	#endif
	//* for FIFO

	//* for LRU
	#ifdef LRU
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->tag[w][req_idx].repl == _cache->way-1)
		{
			victim_way = w;
			break;
		}
	}
	assert(victim_way != UINT32_MAX);
	#endif
	//* for LRU
	
	//* for HITC
	#ifdef HITC
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->tag[w][req_idx].repl == _cache->way-1)
		{
			victim_way = w;
			break;
		}
	}
	assert(victim_way != UINT32_MAX);
	#endif
	//* for HITC

	//* for DIST
	#ifdef DIST
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->tag[w][req_idx].repl == _cache->way-1)
		{
			victim_way = w;
			break;
		}
	}
	assert(victim_way != UINT32_MAX);
	#endif
	//* for DIST

	#ifdef KMEANS
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->tag[w][req_idx].repl == _cache->way-1)
		{
			victim_way = w;
			break;
		}
	}
	assert(victim_way != UINT32_MAX);
	#endif
	//* for KEMANS

	#ifdef HAWKEYE
	//* find replacement victim,
	//* @return value should be 0 ~ 15 or 16 (bypass)

	//* look for the maxRRPV line:
	for (uint32_t w=0; w<_cache->way; w++)
		if (_cache->rrpv[w][req_idx] == H_maxRRPV)
			return w;

	//* If we cannot find a cache-averse line, we evict the oldest cache-friendly line
	uint32_t max_rrip = 0;
	int32_t  lru_victim = -1;
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->rrpv[w][req_idx] >= max_rrip)
		{
			max_rrip = _cache->rrpv[w][req_idx];
			lru_victim = w;
		}
	}
	assert (lru_victim != -1);

	//* The predictor is trained negatively on LRU evictions
	if (SAMPLED_SET(req_idx, 1, _cache->set))
	{
		if (_cache->prefetched[lru_victim][req_idx])
			_cache->prefetch_predictor->decrement(_cache->signatures[lru_victim][req_idx]);
		else
			_cache->demand_predictor->decrement(_cache->signatures[lru_victim][req_idx]);
	}
	return lru_victim;
	#endif


	return victim_way;
}

//* void cache_insert_line(): insert a new cache line (cache block) into appropriate position (way, set)
void cache_insert_line(cache_t* _cache, uint64_t _addr, uint32_t _way, uint64_t _pc)
{
	uint64_t req_tag = (_addr >> (uint64_t)(_cache->index_bit + _cache->offset_bit));
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;

	#ifdef FIFO
	cache_FIFO_replacement(_cache, _addr, _way, false);
	#endif
	#ifdef LRU
	cache_LRU_replacement(_cache, _addr, _way, false);
	#endif
	#ifdef HITC
	cache_HITC_replacement(_cache, _addr, _way, false, _pc);
	#endif
	#ifdef DIST
	cache_DIST_replacement(_cache, _addr, _way, false, _pc);
	#endif
	#ifdef HAWKEYE
	cache_HawkEye_replacement(_cache, _addr, _way, false, false, _pc);
	#endif
	#ifdef KMEANS
	if (DATA_COUNT > 10) {	cache_KMEANS_replacement(_cache, _addr, _way, false, _pc); }
	else {	cache_DIST_replacement(_cache, _addr, _way, false, _pc); }
	#endif

	_cache->tag[_way][req_idx].valid = true;
	_cache->tag[_way][req_idx].tag = req_tag;
	_cache->tag[_way][req_idx].dirty = false;
	_cache->tag[_way][req_idx].used = false;
	_cache->tag[_way][req_idx].pc = _pc;

	_cache->reuse[_way][req_idx].total_acc = 0;
	_cache->reuse[_way][req_idx].data_change++;
}

//* uint64_t cache_evict_line(): determines which cache line should be evicted and initializes the line. it returns the physical memory address of evicted cache line.
uint32_t cache_evict_line(cache_t* _cache, uint64_t _addr)
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	uint32_t way = find_victim_way(_cache, _addr);

	uint64_t evict_addr = _cache->tag[way][req_idx].tag << (_cache->offset_bit + _cache->index_bit) + (req_idx << _cache->offset_bit);

	_cache->tag[way][req_idx].valid = false;

	if (_cache->reuse[way][req_idx].total_acc)
		_cache->reuse[way][req_idx].reuse_hit++;

	return way;
}

void cache_FIFO_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit)
{
	if (is_way_full(_cache, _addr) != -1 || _hit)
		return;
	
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;

	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (w != _way)
		{
			_cache->tag[w][req_idx].repl--;
		}
	}
	_cache->tag[_way][req_idx].repl = _cache->way-1;
}

void cache_LRU_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit)
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;

	for (uint32_t w=0; w<_cache->way; w++)
	{
		if (_cache->tag[w][req_idx].repl < _cache->tag[_way][req_idx].repl)
		{
			_cache->tag[w][req_idx].repl++;
		}
	}
	_cache->tag[_way][req_idx].repl = 0;	//* promote to the MRU position.
}

void cache_DIST_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, uint64_t _pc)
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	bool has_max = false;
	if(!_hit)_cache->tag[_way][req_idx].repl = 1;
	else if(_cache->tag[_way][req_idx].repl > 0)_cache->tag[_way][req_idx].repl--;
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;	
	}
	for(int count = 0; !has_max; count++)
	{	
		for (uint32_t w=0; w<_cache->way; w++)
		{
			if(count > _cache->way || findHashDistance(_cache, _cache->tag[w][req_idx].pc) == NULL || findHashDistance(_cache, _cache->tag[w][req_idx].pc) > (float)_cache->distance_sum / _cache->table_data_num)
			{
			_cache->tag[w][req_idx].repl++;
			}
		}
		for (uint32_t w=0; w<_cache->way; w++)
		{
			if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;
		}
	}
}

void cache_HITC_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, uint64_t _pc)
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	bool has_max = false;
	if(!_hit)_cache->tag[_way][req_idx].repl = 1;//_cache->way - 2;
	else if(_cache->tag[_way][req_idx].repl > 0)_cache->tag[_way][req_idx].repl--;	//* promote to the MRU position.
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;	
	}
	for(int count = 0; !has_max; count++)
	{	
		for (uint32_t w=0; w<_cache->way; w++)
		{
			if(count > _cache->way || findHashData(_cache, _cache->tag[w][req_idx].pc) == NULL || findHashData(_cache, _cache->tag[w][req_idx].pc) < (float)_cache->hitcount_sum / _cache->table_data_num)
			{
			_cache->tag[w][req_idx].repl++;
			}
		}
		for (uint32_t w=0; w<_cache->way; w++)
		{
			if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;
		}
	}
}

void init_cache_replacement(cache_t* _cache)
{
#ifdef HAWKEYE
	_cache->H_SAMPLER_CACHE_SIZE = 2800;// * _cache->_proc->n_cores;
	_cache->H_SHCT_SIZE_BITS = 2; //1 + _cache->_proc->n_cores;
	_cache->H_SHCT_SIZE = 1<<_cache->H_SHCT_SIZE_BITS;
	_cache->SAMPLER_WAY = SAMPLER_WAYS;
	_cache->SAMPLER_SET = _cache->H_SAMPLER_CACHE_SIZE / _cache->SAMPLER_WAY;

	_cache->rrpv = (uint32_t**)malloc(sizeof(uint32_t*) * _cache->way);
	_cache->signatures = (uint64_t**)malloc(sizeof(uint64_t*) * _cache->way);
	_cache->prefetched = (bool**)malloc(sizeof(bool*)*_cache->way);

	for (uint32_t w=0; w<_cache->way; w++)
	{
		_cache->rrpv[w] = (uint32_t*)malloc(sizeof(uint32_t) * _cache->set);
		_cache->signatures[w] = (uint64_t*)malloc(sizeof(uint64_t) * _cache->set);
		_cache->prefetched[w] = (bool*)malloc(sizeof(bool) * _cache->set);
	}

	_cache->perset_mytimer = (uint64_t*)malloc(sizeof(uint64_t) * _cache->set);
	_cache->perset_optgen = (OPTgen*)malloc(sizeof(OPTgen) * _cache->set);
	_cache->addr_history = new std::vector<std::map<uint64_t, ADDR_INFO>>();	

	for (int w=0; w<_cache->way; w++)
	{
		for (int s=0; s<_cache->set; s++)
		{
			_cache->rrpv[w][s] = H_maxRRPV;
			_cache->signatures[w][s] = 0;
			_cache->prefetched[w][s] = false;					
		}
	}

	for (int s=0; s<_cache->set; s++)
	{
		_cache->perset_mytimer[s] = 0;
		_cache->perset_optgen[s].init(_cache->way - 2);
	}

	_cache->addr_history->resize(_cache->SAMPLER_SET);
	for (int s=0; s<_cache->SAMPLER_SET; s++)
		_cache->addr_history->at(s).clear();

	_cache->demand_predictor = new HAWKEYE_PC_PREDICTOR(1);
	_cache->prefetch_predictor = new HAWKEYE_PC_PREDICTOR(1);

#endif
}

void cache_HawkEye_replacement(cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, bool _wb, uint64_t _pc)
{
	uint64_t _set = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	_addr = (_addr >> 6) << 6;

	_cache->prefetched[_way][_set] = false;
	
	//* Ignore writebacks
	if (_wb)
		return;

	//* If we are sampling, OPTgen will only see access from sampled sets
	if (SAMPLED_SET(_set, 1, _cache->set))
	{
		//* The current timestep
		uint64_t curr_quanta = _cache->perset_mytimer[_set] % OPTGEN_VECTOR_SIZE;

		uint32_t sampler_set = (_addr >> 6) % _cache->SAMPLER_SET;
		uint64_t sampler_tag = CRC(_addr >> 12) % 256;
		assert(sampler_set < _cache->SAMPLER_SET);

		//* This line has been used before. Since the right end of a usage interval is always a demand,
		//* ignore prefetches
		if ((_cache->addr_history->at(sampler_set).find(sampler_tag) != _cache->addr_history->at(sampler_set).end()))
		{
			unsigned int curr_timer = _cache->perset_mytimer[_set];
			if (curr_timer < _cache->addr_history->at(sampler_set)[sampler_tag].last_quanta)
				curr_timer = curr_timer + TIMER_SIZE;
			bool wrap = ((curr_timer - _cache->addr_history->at(sampler_set)[sampler_tag].last_quanta) > OPTGEN_VECTOR_SIZE);
			uint64_t last_quanta = _cache->addr_history->at(sampler_set)[sampler_tag].last_quanta % OPTGEN_VECTOR_SIZE;
			//* and for prefetch hits, we train the last prefetch trigger PC
			if (!wrap && _cache->perset_optgen[_set].should_cache(curr_quanta, last_quanta))
			{
				if (_cache->addr_history->at(sampler_set)[sampler_tag].prefetched)
					_cache->prefetch_predictor->increment(_cache->addr_history->at(sampler_set)[sampler_tag].PC);
				else
					_cache->demand_predictor->increment(_cache->addr_history->at(sampler_set)[sampler_tag].PC);
			}
			else
			{
				//* Train the predictor negatively because OPT would not have cached this line
				if (_cache->addr_history->at(sampler_set)[sampler_tag].prefetched)
					_cache->prefetch_predictor->decrement(_cache->addr_history->at(sampler_set)[sampler_tag].PC);
				else
					_cache->demand_predictor->decrement(_cache->addr_history->at(sampler_set)[sampler_tag].PC);
			}
			//* Some maintenance operations for OPTgen
			_cache->perset_optgen[_set].add_access(curr_quanta);
			HawkEye_update_addr_history_lru(_cache, sampler_set, _cache->addr_history->at(sampler_set)[sampler_tag].lru);

			//* Since this was a demand access, mark the prefetched bit as false
			_cache->addr_history->at(sampler_set)[sampler_tag].prefetched = false;
		}
		//* This is the first time we are seeing this line (could be demand or prefetch)
		else if (_cache->addr_history->at(sampler_set).find(sampler_tag) == _cache->addr_history->at(sampler_set).end())
		{
			//* Find a victim from the sampled cache if we are sampling
			if (_cache->addr_history->at(sampler_set).size() == SAMPLER_WAYS)
				HawkEye_replace_addr_history_element(_cache, sampler_set);
		
			assert(_cache->addr_history->at(sampler_set).size() < SAMPLER_WAYS);
			//* Initialize a new entry in the sampler
			_cache->addr_history->at(sampler_set)[sampler_tag].init(curr_quanta);
			//* If it's a prefetch, mark the prefetched bit;
			_cache->perset_optgen[_set].add_access(curr_quanta);
			HawkEye_update_addr_history_lru(_cache, sampler_set, SAMPLER_WAYS-1);
		}
		else	//* This line is a prefetch
		{
			assert(_cache->addr_history->at(sampler_set).find(sampler_tag) != _cache->addr_history->at(sampler_set).end());
			uint64_t last_quanta = _cache->addr_history->at(sampler_set)[sampler_tag].last_quanta % OPTGEN_VECTOR_SIZE;
			if (_cache->perset_mytimer[_set] - _cache->addr_history->at(sampler_set)[sampler_tag].last_quanta < 5 * 1)
			{
				if (_cache->perset_optgen[_set].should_cache(curr_quanta, last_quanta))
				{
					if (_cache->addr_history->at(sampler_set)[sampler_tag].prefetched)
						_cache->prefetch_predictor->increment(_cache->addr_history->at(sampler_set)[sampler_tag].PC);
					else
						_cache->demand_predictor->increment(_cache->addr_history->at(sampler_set)[sampler_tag].PC);
				}
			}
			//* Mark the prefetched bit
			_cache->addr_history->at(sampler_set)[sampler_tag].mark_prefetch();
			//* Some maintenance operations for OPTgen
			_cache->perset_optgen[_set].add_prefetch(curr_quanta);
			HawkEye_update_addr_history_lru(_cache, sampler_set, _cache->addr_history->at(sampler_set)[sampler_tag].lru);
		}

		//* Get Hawkeye's prediction for this line
		bool new_prediction = _cache->demand_predictor->get_prediction(_pc);

		//* Update the sampler with the timestamp, PC and our prediction
		//* For prefetches, the PC will represent the trigger PC
		_cache->addr_history->at(sampler_set)[sampler_tag].update(_cache->perset_mytimer[_set], _pc, new_prediction);
		_cache->addr_history->at(sampler_set)[sampler_tag].lru = 0;
		//* Increment the set timer
		_cache->perset_mytimer[_set] = (_cache->perset_mytimer[_set] +1) % TIMER_SIZE;
	}


	bool new_prediction = _cache->demand_predictor->get_prediction(_pc);

	_cache->signatures[_way][_set] = _pc;

	//* Set RRIP values and age cache-friendly line
	if (!new_prediction)
		_cache->rrpv[_way][_set] = H_maxRRPV;
	else
	{
		_cache->rrpv[_way][_set] = 0;
		if (!_hit)
		{
			bool saturated = false;
			for (uint32_t w=0; w<_cache->way; w++)
				if (_cache->rrpv[w][_set] == H_maxRRPV-1)
					saturated = true;
		
			//* Age all the cache-friendly lines
			for (uint32_t w=0; w<_cache->way; w++)
			{
				if (!saturated && _cache->rrpv[w][_set] < H_maxRRPV - 1)
					_cache->rrpv[w][_set]++;
			}
		}
		_cache->rrpv[_way][_set] = 0;
	}

}

void cache_KMEANS_replacement (cache_t* _cache, uint64_t _addr, uint32_t _way, bool _hit, uint64_t _pc)
{
	uint64_t req_idx = (_addr >> (uint64_t)(_cache->offset_bit)) % (uint64_t)_cache->set;
	bool has_max = false;
	if(!_hit)_cache->tag[_way][req_idx].repl = 1;//_cache->way - 2;
	else if(_cache->tag[_way][req_idx].repl > 0)_cache->tag[_way][req_idx].repl--;	//* promote to the MRU position.
	for (uint32_t w=0; w<_cache->way; w++)
	{
		if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;	
	}
	for(int count = 0; !has_max; count++)
	{	
		for (uint32_t w=0; w<_cache->way; w++)
		{
			bool flag = count > _cache->way || findHashData(_cache, _cache->tag[w][req_idx].pc) == NULL;
			if(!flag && k0_DATA_COUNT > 0 && k1_DATA_COUNT > 0)
			{
				if( pow(k0_x / k0_DATA_COUNT - findHashData(_cache, _cache->tag[w][req_idx].pc), 2) + 
						pow(k0_y / k0_DATA_COUNT - findHashDistance(_cache, _cache->tag[w][req_idx].pc), 2)
						> pow(k1_x / k1_DATA_COUNT - findHashData(_cache, _cache->tag[w][req_idx].pc), 2) + 
						pow(k1_y / k1_DATA_COUNT - findHashDistance(_cache, _cache->tag[w][req_idx].pc), 2))
				{
					if(k1_x / k1_DATA_COUNT > k0_x / k0_DATA_COUNT)flag = true;
				}
				else
				{
					if(k1_x / k1_DATA_COUNT < k0_x / k0_DATA_COUNT)flag = true;
				}
			}
			if(flag)
			{
			_cache->tag[w][req_idx].repl++;
			}
		}
		for (uint32_t w=0; w<_cache->way; w++)
		{
			if(_cache->tag[w][req_idx].repl == _cache->way - 1) has_max = true;
		}
	}
}
