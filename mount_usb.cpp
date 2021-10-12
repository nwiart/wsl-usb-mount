/*
 *
 */

#include <iostream>
#include <string>

#include <string.h>

using namespace std;



#define VERSION "1.0"

#define LOGINFO( x ) cout << "[INFO]: "  << x << endl;
#define LOGWARN( x ) cout << "[WARN]: "  << x << endl;
#define LOGERR( x )  cout << "[ERROR]: " << x << endl;

void help();

char to_lower( char c )
{
	if ( c >= 'A' && c <= 'Z' )
		return c - 'A' + 'a';
	return c;
}



int main( int argc, char** argv )
{
	if ( argc == 1 )
	{
		help();
		return 0;
	}

	// Parse options.
	// "i" is reused to indicate the drive letter argument.
	bool unmount = false;
	int i = 1;
	for ( i; i < argc; ++i )
	{
		if ( argv[i][0] == '-' )
		{
			if ( argv[i][1] == '-' )
			{
				// Help command.
				if ( strncmp( argv[i], "--help", 6 ) == 0 )
				{
					help();
					return 0;
				}
			}
			else
			{
				switch ( argv[i][1] )
				{
				case 'u':
					unmount = true;
					break;
				}
			}
		}
		else
		{
			break;
		}
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

		if ( unmount )
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

void help()
{
	cout << "\nUsbMount : Vous permet de mount / unmount facilement un stockage USB dans un environnement WSL.\n";
	cout << "Original Author : David Kroft\n\n";
	cout << "Version : " << VERSION << "\n\n";

	cout << "Usage : sudo ./usbmount [--help] [-u] <windows_drive_letter>\n\n";

	// Command list (TODO : automate with an iterator).
	cout << "	--help : Affiche ce message. Toutes les autres options sont ignorées.\n\n";

	cout << "	-u : Doit démonter le périphérique spécifié.\n\n";
}
