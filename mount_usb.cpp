/*
 * UsbMount : USB storage device mounting / unmounting utility for WSL.
 *
 * Created on October 12th 2021.
 * Original Author : David Kroft
 */

#include <iostream>
#include <string>

#include <string.h>

using namespace std;



#define VERSION "1.01"

#define LOGINFO( x ) cout << "[INFO]: "  << x << endl;
#define LOGWARN( x ) cout << "[WARN]: "  << x << endl;
#define LOGERR( x )  cout << "[ERROR]: " << x << endl;



struct option
{
	char        name_short; // Ex: 'h' for "-h"
	const char* name_long;  // Ex: "--help"
	const char* desc;
};

void init_options();

bool is_specified( char option );
void set_specified( char option, bool sp );

void cmd_help();
void cmd_version();

option options[] =
{
	{ 'h', "--help", "Displays this message. All other options are ignored." },
	{ 'V', "--version", "Displays the current version. All other options are ignored." },
	{ 'u', "--unmount", "Tells UsbMount to unmount an existing USB media rather than mounting it. The program assumes that the media was mounted using UsbMount as well." }
};

// Array of specified options.
bool* specified_options = 0;

char to_lower( char c )
{
	if ( c >= 'A' && c <= 'Z' )
		return c - 'A' + 'a';
	return c;
}



int main( int argc, char** argv )
{
	init_options();

	// No arguments, so we just display help then quit.
	if ( argc == 1 )
	{
		cmd_help();
		return 0;
	}

	// Parse options.
	// "i" is reused to indicate the drive letter argument.
	bool unmount = false;
	int i = 1;
	for ( i; i < argc; ++i )
	{
		// Option!
		if ( argv[i][0] == '-' )
		{
			// Long option (double hyphen).
			if ( argv[i][1] == '-' )
			{
				// Help command.
				if ( strncmp( argv[i], "--help", 6 ) == 0 ) {
					cmd_help();
					return 0;
				}

				// Version command.
				else if ( strncmp( argv[i], "--version", 9 ) == 0 ) {
					cmd_version();
					return 0;
				}

				// Unmount.
				else if ( strncmp( argv[i], "--unmount", 9 ) == 0 )
					set_specified( 'u', true );
			}

			// Single-character option.
			else
			{
				// Option combining ("-a -b -c" == "-abc").
				int c_index = 1;
				while ( argv[i][c_index] )
				{
					char option_c = argv[i][c_index];
					if ( isalnum( option_c ) )
						specified_options[argv[i][c_index]] = true;
					else
						LOGWARN( "Invalid option character : " << option_c << ". This is ignored." );

					c_index++;
				}
			}
		}
		else
		{
			break;
		}
	}

	if ( is_specified( 'h' ) )
	{
		cmd_help();
		return 0;
	}
	if ( is_specified( 'V' ) )
	{
		cmd_version();
		return 0;
	}

	// Check if drive letter is last argument.
	if ( i == argc )
	{
		LOGERR( "You must specify a drive letter." );
		return 2;
	}

	// Now execute the commands!
	char drive_letter = to_lower( argv[i][0] );
	if ( drive_letter >= 'a' && drive_letter <= 'z' )
	{
		string cmd;

		if ( is_specified( 'u' ) )
		{
			LOGINFO( "Unmount command issued." );
			cmd += "umount /mnt/"; cmd += drive_letter;
		}
		else
		{
			LOGINFO( "Mount command issued." );

			// Make mount directory command.
			cmd += "mkdir /mnt/"; cmd += drive_letter; cmd += "; ";

			// Actual mount command.
			cmd += "mount -t drvfs "; cmd += drive_letter; cmd += ": /mnt/"; cmd += drive_letter;
		}

		system( cmd.c_str() );
	}
	else
	{
		LOGERR( "Drive letter is invalid." );
		return 1;
	}
}



// Initializes the array of specified options.
void init_options()
{
	unsigned int size = sizeof(options) / sizeof(option);

	specified_options = (bool*) malloc( size );
	memset( specified_options, 0, size );
}

// Returns whether an option was explicitely specified in the command line.
// "option" is the case-sensitive alphanumeric character which identifies an option.
bool is_specified( char option )
{
	return specified_options[option];
}

// Updates the specified state of an option.
void set_specified( char option, bool sp )
{
	specified_options[option] = sp;
}

void cmd_help()
{
	cout << "\nUsbMount : Makes mounting / unmounting USB medias in a WSL environment easy!\n";
	cout << "Original Author : David Kroft\n\n";
	cout << "Version : " << VERSION << "\n\n";

	cout << "Usage : sudo ./usbmount [options] [windows_drive_letter]\n\n";

	// Command list (TODO : automate with an iterator).
	for ( int i = 0; i < sizeof(options) / sizeof(option); ++i )
	{
		option* o = &options[i];

		// Short option name.
		cout << "\t-";
		cout << o->name_short << ", ";

		// Long option name.
		cout << o->name_long << " : ";

		// Option description.
		cout << (o->desc ? o->desc : "MISSING DESCRIPTION") << "\n\n";
	}
}

void cmd_version()
{
	cout << "Version : " << VERSION << "\n";
}
