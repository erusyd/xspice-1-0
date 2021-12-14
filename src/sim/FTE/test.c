
main()
{

	char buf[32];
	int i;

	for (i=0; i < 32; i++) buf[i] = '\0';
	sprintf(buf, "%1.1e", 0.0);
	printf("%s*** %d\n", buf, strlen(buf));

}
