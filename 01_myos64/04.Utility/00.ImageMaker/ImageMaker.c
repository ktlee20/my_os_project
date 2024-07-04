#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BYTESOFSECTOR 512

int adjust_sector_size (int fd, int source_size);
void write_kernel_information (int target_fd, int kernel_sector_count);
int copy_file(int source_fd, int target_fd);

int
main(int argc, char *argv[])
{
	int source_fd;
	int target_fd;
	int boot_loader_size;
	int kernel32_sector_count;
	int source_size;

	// Check command line options
	if (argc < 3) {
		fprintf(stderr, "[ERROR] ImageMaker BootLoader.bin Kernel32.bin\n");
		exit(-1);
	}

	// Create Disk.img
	if ((target_fd = open("Disk.img", O_RDWR|O_CREAT|O_TRUNC,
			S_IREAD|S_IWRITE)) == -1) {
		fprintf(stderr, "[ERROR] Disk.img open fail.\n");
		exit(-1);
	}

	// Copy boot loader to Disk.img
	printf( "[INFO] Copy boot loader to image file\n" );
	if ((source_fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "[ERROR] %s open fail\n", argv[1]);
		exit(-1);
	}

	source_size = copy_file(source_fd, target_fd);
	close(source_fd);

	// Padding 0x00 for 512-bytes alignments
	boot_loader_size = adjust_sector_size(target_fd, source_size);
	printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[1],
			source_size, boot_loader_size);

	// Copy 32-bit kernel file to disk image file
	printf("[INFO] Copy protected mode kernel to image file\n");
	if ((source_fd = open (argv[2], O_RDONLY)) == -1) {
		fprintf(stderr, "[ERROR] %s open fail\n", argv[2]);
		exit(-1);
	}

	source_size = copy_file(source_fd, target_fd);
	close(source_fd);

	// Padding 0x00 for 512-bytes alignments
	kernel32_sector_count = adjust_sector_size(target_fd, source_size);
	printf("[INFO] %s size = [%d] and sector count = [%d]\n",
			argv[2], source_size, kernel32_sector_count);

	printf("[INFO] Start to write kernel information\n");
	write_kernel_information(target_fd, kernel32_sector_count);
	printf("[INFO] Image file create complete\n");

	close(target_fd);
	return 0;
}

int adjust_sector_size(int fd, int source_size)
{
	int i;
	int adjust_size_to_sector;
	char zero_pad;
	int sector_count;

	adjust_size_to_sector = source_size % BYTESOFSECTOR;
	zero_pad = 0x00;

	if (adjust_size_to_sector != 0) {
		adjust_size_to_sector = 512 - adjust_size_to_sector;
		printf("[INFO] File size [%d] and full [%d] byte\n", source_size,
				adjust_size_to_sector);
		for (i = 0 ; i < adjust_size_to_sector ; i++) {
			write(fd, &zero_pad, 1);
		}
	} else {
		printf("[INFO] File size is aligned 512 bytes\n");
	}

	sector_count = (source_size + adjust_size_to_sector) / BYTESOFSECTOR;
	return sector_count;
}

void write_kernel_information(int target_fd, int kernel_sector_count)
{
	unsigned short data;
	long position;

	position = lseek(target_fd, (off_t)5, SEEK_SET);
	if (position == -1) {
		fprintf(stderr, "lseek fail. Return value = %ld, errno = %d, %d\n",
				position, errno, SEEK_SET);
		exit(-1);
	}

	data = (unsigned short) kernel_sector_count;
	write (target_fd, &data, 2);

	printf("[INFO] Total sector count except boot loader [%d]\n",
			kernel_sector_count);
}

int copy_file(int source_fd, int target_fd)
{
	int source_file_size;
	int read_size;
	int write_size;
	char buffer[BYTESOFSECTOR];

	source_file_size = 0;

	while (1) {
		read_size = read(source_fd, buffer, sizeof(buffer));
		write_size = write(target_fd, buffer, read_size);

		if (read_size != write_size) {
			fprintf(stderr, "[ERROR] read_size != write_size.. \n");
			exit(-1);
		}
		source_file_size += read_size;

		if (read_size != sizeof(buffer)) {
			break;
		}
	}
	return source_file_size;
}
