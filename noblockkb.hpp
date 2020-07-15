    #ifndef __NOBLOCKKB__
    #define __NOBLOCKKB__
    
    /*
        NoBlockKB - "Non-Blocking Keyboard Input" 

          A simple class that has a few functions to use non-blocking keyboard input for terminal and console
        applications. Unlike Windows, Linux does not come with a simple way to do this and I got tired of 
        re-programming this same code. So I packaged it and threw it up on github. Hope it helps someone, somewhere!

            The class does have support for Windows so you don't have to change code around if you swap platforms, HOWEVER,
        it is completely untested -- ... But it should work.

        original author: Michael Scott Stuart (mikeypro83)
        author email: (professional inquiries only) michaelscottstuart at yahoo dot com
    */
   
    #ifdef __unix__
    #include "termios.h"
    #include <fcntl.h>
    #endif

    #ifdef _WINDOWS
    #include "windows.h"
    #endif

    class cInput    
    {
        public:            

            cInput(void)
            {
                m_lastCh = -1;
            }

            virtual ~cInput(void)
            {

            }

            virtual bool isKey(void)
            {
                #ifdef _WINDOWS
                    return kbhit();
                #endif
                #ifdef __unix__
                    return (getCh()!=-1);    
                #endif
            }

            virtual int getCh(void)
            {
                #ifdef _WINDOWS                    
                    return (kbhit()?getc():-1);                    
                #endif 
                #ifdef __unix__
                    termios ocfg, ncfg;
                    tcgetattr(0, &ocfg);
                    ncfg = ocfg;
                    ncfg.c_lflag &= ~ICANON;	// mark non-canonical
                    ncfg.c_lflag &= ~ECHO;		// turn off echo
                    //ncfg.c_lflag &= ~ISIG;	// commented so user can ctrl-c out of program in unix.
                        
                    tcsetattr(0, TCSANOW, &ncfg);		
                    int ch = -1;
                    int cfg = fcntl(0, F_GETFL);
                    fcntl(0, F_SETFL, cfg | O_NONBLOCK);
                    ch = getchar();
                    fcntl(0, F_SETFL, cfg);
                    tcsetattr(0, TCSANOW, &ocfg); // reset terminal input handling.      
                    m_lastCh = ch; // -1 is valid
                    return ch;
                #endif // __unix__

            }

        private:

            int m_lastCh;
    }; // class cInput

    #endif