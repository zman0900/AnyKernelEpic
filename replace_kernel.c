#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 7) {
		printf("usage: %s old_bootimg boot_offset boot_len recovery_len new_kernel new_bootimg\n",
		       argv[0]);
	exit(1);
	}
	int boot_offset = atoi(argv[2]);
	int boot_len = atoi(argv[3]);
	int recov_len = atoi(argv[4]);
	
	void *buf;

	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("couldn't open old boot.img\n");
		exit (1);
	}
	FILE *kernel = fopen(argv[5], "r");
	if (kernel == NULL) {
		printf("couldn't open new kernel\n");
		exit (1);
	}
	FILE *out = fopen(argv[6], "w");
	if (in == NULL) {
		printf("couldn't open new boot.img\n");
		exit (1);
	}
	
	// new kernel size
	fseek(kernel, 0, SEEK_END);
	int  kSize = ftell(kernel);
	rewind(kernel);
	int kBlocks;
	if (kSize % 512 == 0) {
		kBlocks = kSize / 512;
	} else {
		kBlocks = kSize / 512 + 1;
	}
	printf("old kernel: %d blocks\nnew kernel: %d blocks", boot_offset - 1,
	       kBlocks);

	// Skip old kernel and header
	fseek(in, 512 * boot_offset, SEEK_SET);
	
	// Read new kernel **Note the calloc to fill end with zero
	buf = calloc(kBlocks + 1, 512);
	size_t count = fread(buf, 512, kBlocks, kernel);
	if (count != kBlocks && count + 1 != kBlocks) {
		printf("failed reading new kernel\n");
		exit(1);
	}
	fclose(kernel);
	
	// Write new kernel + space for header
	fwrite(buf, 512, kBlocks + 1, out);
	free(buf);
	
	// Write header
	fseek(out, -512, SEEK_END);
	fprintf(out, "\n\nBOOT_IMAGE_OFFSETS\n");
	fprintf(out, "boot_offset=%d;boot_len=%d;", kBlocks, boot_len);
	fprintf(out, "recovery_offset=%d;recovery_len=%d;", kBlocks + 1 + boot_len,
	        recov_len);
	fprintf(out, "\n\n");
	fseek(out, 0, SEEK_END);
	
	// Read boot
	buf = malloc(512 * boot_len);
	fread(buf, 512, boot_len, in);
	
	// Write boot
	fwrite(buf, 512, boot_len, out);
	free(buf);
	
	// Read recovery
	buf = malloc(512 * recov_len);
	fread(buf, 512, recov_len, in);
	
	// Write recovery
	fwrite(buf, 512, recov_len, out);
	free(buf);
	
	// Done
	fclose(in);
	fclose(out);
	printf("success\n");
	return 0;
}
