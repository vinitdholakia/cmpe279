#include <stdio.h>
#include <sys/prctl.h>
#include <seccomp.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	char * noargv[] = {NULL};

	scmp_filter_ctx ctx;

	printf("1. Init filter\n");

	ctx = seccomp_init(SCMP_ACT_TRAP);

	


	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(ioctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(bsdthread_register), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sysctlbyname), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(issetugid),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getentropy),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(csops),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat64),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(proc_info),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(socket),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(connect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getrlimit),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat64),0);
	seccomp_load(ctx);

	printf("2. Trying to exec a shell\n");

	execvp("/bin/bash", noargv);

	printf("3. Shell executed\n");

	return 0;
}