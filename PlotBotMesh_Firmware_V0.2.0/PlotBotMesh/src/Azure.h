#ifndef Azure_h
#define Azure_h

	void sendInfoToAzure();
	void sendConfigToAzure();
	#if PLATFORM_ID == PLATFORM_ARGON
		void XenonDataHandler(const char*, const char*);
	#elif PLATFORM_ID == PLATFORM_XENON
		void ArgonDataHandler(const char*, const char*);
	#endif
#endif