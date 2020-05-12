#ifndef GDT_H
#define GDT_H

namespace os
{
	class GDT {
		private:
			GDT();
			~GDT();
		public:
			//TO-DO, move Init to constructor once
			//constructors are working 
			void Init();
			static GDT& get(){
				static os::GDT instance;
				return instance;
			};
	};
}

#endif
