//////////////////////////////////////////////////////////////
//Generator for a static tree of nchar to quick search command
//It seems that there is a great increase of size of index if
//the charset is UNICODE because a lot of UNICODE characters are
//not used in commandline but is still created. (from 2mb of output
//to 31kb of output). If the commands are restricted to the alpha-
//numeric, the size can further decrease (a lot). I guess I'll
//use a macro and make some support within this project

#include "Header.h"
#include "cmd_struct.h"
#include "../Lib/DS_CharTree.h"
#include <queue>

using namespace std;

helper_tree tree;

ifstream ifs;
ofstream ofs;

void default_open() {
	ifs.open("macro_commands.h");
	ofs.open("commands.h");
	if (!ifs || !ofs) {
		throw runtime_error("Cannot open io files!");
	}
	string line;
	while(!ifs.eof()&&ifs) {
		getline(ifs, line);
		istringstream iss{ line };
		string preprocessor; string command; int command_number;
		iss >> preprocessor >> command >> command_number;
		if(preprocessor=="#define") {
#ifdef _UNICODE
			const nstring temp = convert::string2wstring(command);
			tree.add_token(temp);
			tree.access(temp) = command_number;
#endif
#ifdef _MBCS
			tree.add_token(command);
			tree.access(command) = command_number;
#endif
		}
	}
	tree.make_static_list(ofs);
	ifs.close();
	ofs.close();
}

int main() {
	default_open();
	return 0;
}

void helper_tree::make_static_list(std::ostream& ost) {
	size_t cur_index = 0;
	node* cur_node = head;
	queue<node*> visit_queue;
	queue<cmd_page<nchar>*> data;
	num_of_node = 0;
	 do {
		 if(!visit_queue.empty()) visit_queue.pop();
		 if(cur_node) {
			 cmd_page<nchar>* temp = new cmd_page<nchar>;
			 for(size_t i=0; i<find_MAX<nchar>(); i++) {
				 temp->data[i].next = 0;
				 temp->data[i].cmd = 0;
			 }
			 bool push_node = false;
			 if(cur_node->next) {
				 size_t num_of_new_nodes = 0; //number of new nodes to be pushed before the current one
				 for (size_t i = 0; i < find_MAX<nchar>(); i++) {
					 if ((cur_node->next[i])) {
						 push_node = true;
						 num_of_new_nodes++;
						 visit_queue.push(cur_node->next[i]);
						 temp->data[i].cmd = unsigned int(cur_node->next[i]->data);
						 if(cur_node->next[i]->next) {
							 temp->data[i].next = int((num_of_node + num_of_new_nodes)*find_MAX<nchar>());
						 }
					 }
				 }
			 }
			 if(push_node) {
				 num_of_node++;
				 data.push(temp);
			 }
			 else {
				delete temp;
				temp = nullptr;
			}
		 }
		 if(!visit_queue.empty()) {
			 cur_node = visit_queue.front();
		 }
	}while (!visit_queue.empty());
	cmd_page<nchar>* full_list = new cmd_page<nchar>[num_of_node];
	for(size_t i=0; i<num_of_node; i++) {
		const cmd_page<nchar>* temp = data.front();
		data.pop();
		for(size_t j=0; j<find_MAX<nchar>(); j++) {
			full_list[i].data[j].next = temp->data[j].next;
			full_list[i].data[j].cmd = temp->data[j].cmd;
		}
		delete temp;
	}
	ost << "#pragma once\n";
	ost << "#include \"../CmdTreeGen/cmd_struct.h\"" << endl;
	ost << "cmd_node commands["<< num_of_node*find_MAX<nchar>() <<"] {\n";
	for(size_t i=0; i<num_of_node; i++) {
		for(size_t j=0; j<find_MAX<nchar>(); j++) {
			ost << "\t{ " << full_list[i].data[j].next <<", "<< full_list[i].data[j].cmd << " },";
			ost << endl;
		}
	}
	ost << "};\n" << endl;
}

