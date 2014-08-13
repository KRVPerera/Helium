#include "meminfo.h"
#include "moduleinfo.h"
#include <fstream>
#include <vector>
#include "utilities.h"
#include "memlayout.h"

using namespace std;


/* read the files and get the mem information recorded */

vector<pc_mem_region_t *> get_mem_regions_from_memtrace(vector<ifstream *> &memtrace, moduleinfo_t * head){

	vector<pc_mem_region_t *> pc_mems;

	for (int i = 0; i < memtrace.size(); i++){
		while (!(*memtrace[i]).eof()){

			string line;
			getline((*memtrace[i]), line);

			if (!line.empty()){
				vector<string> tokens = split(line, ',');

				mem_input_t * input = new mem_input_t;

				input->module = find_module(head, strtoull(tokens[0].c_str(), NULL, 16))->name;
				input->pc = strtoul(tokens[1].c_str(), NULL, 16);
				input->write = tokens[2][0] - '0';
				input->stride = atoi(tokens[3].c_str());
				input->type = MEM_HEAP_TYPE;
				input->mem_addr = strtoull(tokens[4].c_str(), NULL, 16);

				update_mem_regions(pc_mems, input);

				delete input;
			}


		}
	}

	postprocess_mem_regions(pc_mems);

	return pc_mems;

}

/* now create the composition layout */
func_composition_t * find_func_composition(vector<func_composition_t *> &funcs, string module, uint32_t addr){
	for (int i = 0; i < funcs.size(); i++){
		if (funcs[i]->module_name.compare(module) == 0 && addr == funcs[i]->func_addr){
			return funcs[i];
		}
	}
	return NULL;
}


vector<func_composition_t *> create_func_composition(vector<pc_mem_region_t *> &regions, moduleinfo_t * head){

	vector<func_composition_t *>  funcs;

	for (int i = 0; i < regions.size(); i++){
		moduleinfo_t * module = find_module(head, regions[i]->module);
		if (module != NULL){
			funcinfo_t * func = find_func_app_pc(module, regions[i]->pc);
			if (func != NULL){
				func_composition_t * comp = find_func_composition(funcs, regions[i]->module, func->start_addr);
				if (comp == NULL){
					comp = new func_composition_t;
					comp->module_name = regions[i]->module;
					comp->func_addr = func->start_addr;
					funcs.push_back(comp);
				}

				comp->region.push_back(regions[i]);
			}
		}
	}

	return funcs;
}


void print_app_pc_info(ofstream &file, vector<func_composition_t *> &funcs){

	file << funcs.size() << endl;
	for (int i = 0; i < funcs.size(); i++){
		file << funcs[i]->module_name << endl;
		file << funcs[i]->func_addr << endl;
		file << funcs[i]->region.size() << endl;
		for (int j = 0; j < funcs[i]->region.size(); j++){
			file << funcs[i]->region[j]->pc << endl;
		}
	}

}