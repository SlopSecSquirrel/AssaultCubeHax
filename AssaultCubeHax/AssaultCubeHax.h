// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ASSAULTCUBEHAX_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ASSAULTCUBEHAX_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ASSAULTCUBEHAX_EXPORTS
#define ASSAULTCUBEHAX_API __declspec(dllexport)
#else
#define ASSAULTCUBEHAX_API __declspec(dllimport)
#endif

// This class is exported from the dll
class ASSAULTCUBEHAX_API CAssaultCubeHax {
public:
	CAssaultCubeHax(void);
	// TODO: add your methods here.
};

extern ASSAULTCUBEHAX_API int nAssaultCubeHax;

ASSAULTCUBEHAX_API int fnAssaultCubeHax(void);
