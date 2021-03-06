#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../memory/kheap.h"

char shell_name[15] = "SHELL";
extern char color_attribute;
enum vga_color color_vga;

void print_SAY_OS();

/* Commands declaration */
void command_help(char *input);
void command_name(char *input);
void command_color(char *input);
void command_picture(char *input);

void init_header()
{
	kprint_middle_row("SAY OS KERNEL SHELL (type help to get command name)\n\n", 0);
	fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
}

void kshell()
{
	color_attribute = (char)vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	color_vga = VGA_COLOR_CYAN;
	init_header();
	print_SAY_OS();
	kprint(shell_name);
	kprint("> ");
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
	else if (strcmp(command, "PICTURE") == 0)
	{
		command_picture(input);
	}

	kfree(command);
	kprint(shell_name);
	kprint("> ");
}

void print_SAY_OS()
{
	kprint("\n");
	kprint("		  /$$$$$$   /$$$$$$  /$$     /$$        /$$$$$$   /$$$$$$ \n");
	kprint("		 /$$__  $$ /$$__  $$|  $$   /$$/       /$$__  $$ /$$__  $$\n");
	kprint("		| $$  \\__/| $$  \\ $$ \\  $$ /$$/       | $$  \\ $$| $$  \\__/\n");
	kprint("		|  $$$$$$ | $$$$$$$$  \\  $$$$/        | $$  | $$|  $$$$$$ \n");
	kprint("		 \\____  $$| $$__  $$   \\  $$/         | $$  | $$ \\____  $$\n");
	kprint(" 		/$$  \\ $$| $$  | $$    | $$          | $$  | $$ /$$  \\ $$\n");
	kprint("		|  $$$$$$/| $$  | $$    | $$          |  $$$$$$/|  $$$$$$/\n");
	kprint(" 		\\______/ |__/  |__/    |__/           \\______/  \\______/\n");
	kprint("\n");
}

void command_help(char *input)
{
	char *help = strtok(input, ' ', 1);

	if (!help)
		kprint("\tList of commands\n\tNAME END CLEAR COLOR HELP PICTURE\n\tType `HELP NAME` to find more about command NAME.\n");
	else if (strcmp(help, "NAME") == 0)
	{
		kprint("\tNAME : Give 'name' argument to change shell name.\n");
	}
	else if (strcmp(help, "COLOR") == 0)
	{
		kprint("\tCOLOR : Give 'color' argument to change shell color.\n");
		kprint("\tOptions : Red, Green, Blue, Cyan, White\n");
	}
	else if (strcmp(help, "CLEAR") == 0)
	{
		kprint("\tCLEAR : Clears the screen.\n");
	}
	else if (strcmp(help, "END") == 0)
	{
		kprint("\tEND : Halts the CPU.\n");
	}
	else if (strcmp(help, "PICTURE") == 0)
	{
		kprint("\tPICTURE : Prints a picture\n");
		kprint("\tAvailable picture: RAIN\n");
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
	}
	kfree(name);
}

void command_color(char *input)
{
	char *color = strtok(input, ' ', 1);

	if (!color)
		kprint("1 Argument Required!\n");
	else if (strcmp(color, "RED") == 0)
	{
		color_attribute = (char)vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
		color_vga = VGA_COLOR_RED;
		change_text_color((uint8_t)color_attribute);
		fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
	}
	else if (strcmp(color, "GREEN") == 0)
	{
		color_attribute = (char)vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
		color_vga = VGA_COLOR_GREEN;
		change_text_color((uint8_t)color_attribute);
		fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
	}
	else if (strcmp(color, "BLUE") == 0)
	{
		color_attribute = (char)vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
		color_vga = VGA_COLOR_BLUE;
		change_text_color((uint8_t)color_attribute);
		fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
	}
	else if (strcmp(color, "CYAN") == 0)
	{
		color_attribute = (char)vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
		color_vga = VGA_COLOR_CYAN;
		change_text_color((uint8_t)color_attribute);
		fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
	}
	else if (strcmp(color, "WHITE") == 0)
	{
		color_attribute = (char)vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
		color_vga = VGA_COLOR_WHITE;
		change_text_color((uint8_t)color_attribute);
		fill_row_with_color(vga_entry_color(VGA_COLOR_WHITE, color_vga), 0);
	}
	kfree(color);
}

void command_picture(char *input)
{
	char *picture = strtok(input, ' ', 1);
	if (!picture)
	{
		print_SAY_OS();
	}
	else if (strcmp(picture, "RAIN") == 0)
	{
		kprint("      __   _\n");
		kprint("    _(  )_( )_\n");
		kprint("   (_   _    _)\n");
		kprint("  / /(_) (__)\n");
		kprint(" / / / / / /\n");
		kprint("/ / / / / /\n");
	}

	kfree(picture);
}