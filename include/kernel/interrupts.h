#ifndef INTERRUPTS_H
#define INTERRUPTS_H

namespace os {
	namespace Interrupts {
		void Init();
		void EnableInterrupts();
		void DisableInterrupts();

		bool IsEnabled();
		bool IsInitialised();
	}
}

#endif
