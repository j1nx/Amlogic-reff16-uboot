#define	DEBUG
#ifdef DEBUG
#define UPGRADE_DPRINT(args...)	printf(args)
#else
#define UPGRADE_DPRINT(args...)
#endif
#define UPGRADE_ERROR(arg)		printf(arg)
