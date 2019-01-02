#ifndef Cloud_h
#define Cloud_h

	void sendInfoToCloud();
	void sendConfigToCloud();
	#if PLATFORM_ID == PLATFORM_ARGON
		void XenonDataHandler(const char*, const char*);
	#elif PLATFORM_ID == PLATFORM_XENON
		void ArgonDataHandler(const char*, const char*);
	#endif
#endif