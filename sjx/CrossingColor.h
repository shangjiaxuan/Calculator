#pragma once

#include "Crossing.h"

namespace UJr2_funcs {
	namespace crossing {
		class Color {
		public:
			Color() {
				rel = nullptr;
				group = nullptr;
				color_num = 0;
				throw std::runtime_error("UpColors: Please specify a relation to use this class!");
			}

			Color(crossing::relations& rela) {
				color_num = 0;
				rel = &rela;
				group = new int[crossing::possi](); //��0��ʼ������
				//		for (int i = 0; i < crossing::possi; i++) {
				//			group[i] = 0;
				//		}
			}

			~Color() {
				delete[] group;
				group = nullptr;
			}

			void find_groups();
			void print_result();
		protected:
			crossing::relations* rel;
			int color_num;
			int* group;                            //��struct route��Ӧ�ķ�������
			bool evaluate(int cur, int opp) const; //��ĳ��Ԫ���ܷ����Ŀǰ����
			void assign(int add);                  //��ĳ��Ԫ�ط���һ����
		};
	}
}
