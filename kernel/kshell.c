#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../memory/kheap.h"

char shell_name[15] = "SHELL";
extern char color_attribute;
enum vga_color color_vga;

/* Commands declaration */
void command_help(char *input);
void command_name(char *input);
void command_color(char *input);

void init_header()
{
	kprint_middle_row("OS KERNEL (type help to get command name)\n\n", 0);
	fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
}

void kshell()
{
	init_header();
	kprint(shell_name);
	kprint(">");
}

void user_input(char *input)
{
	char *command = strtok(input, ' ', 0);

	if (strcmp(input, "END") == 0)
	{
		kprint("Stopping the CPU. Bye!\n");
		asm volatile("hlt");
	}
	else if (strcmp(input, "CLEAR") == 0)
	{
		clear_screen();
		init_header();
	}
	else if (strcmp(command, "NAME") == 0)
	{
		command_name(input);
	}
	else if (strcmp(command, "HELP") == 0)
	{
		command_help(input);
	}
	else if (strcmp(command, "COLOR") == 0)
	{
		command_color(input);
	}

	kfree(command);
	kprint(shell_name);
	kprint("> ");
}

void command_help(char *input)
{
	char *help = strtok(input, ' ', 1);

	if (!help)
		kprint("\tList of commands\n\tNAME END CLEAR HELP\n\tType `HELP NAME` to find more about command NAME\n");
	else if(strcmp(help, "NAME") == 0) {
		kprint("\tNAME : GIVE ONE ARGUMENT TO CHANGE SHELL NAME\n");
	}

	kfree(help);
}

void command_name(char *input)
{
	char *name = strtok(input, ' ', 1);

	if (!name)
		kprint("1 Argument Required!\n");
	else
	{
		memory_copy(name, shell_name, strlen(name));
		shell_name[strlen(name)] = '\0';
		kfree(name);
	}
}

void command_color(char *input)
{
	char *color = strtok(input, ' ', 1);

	if (!color)
		kprint("1 Argument Required!\n");
	else if(strcmp(color, "RED") == 0)
	{
		color_attribute = (char)vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
		color_vga = VGA_COLOR_RED;
		kfree(color);
	}
}