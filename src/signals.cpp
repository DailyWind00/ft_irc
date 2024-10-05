/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallais <mgallais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 18:27:01 by mgallais          #+#    #+#             */
/*   Updated: 2024/09/23 10:20:45 by mgallais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

struct Signal {
    int number;
    const char* name;
};

const Signal signals[] = {
    {SIGHUP, "SIGHUP"},
    {SIGINT, "SIGINT"},
    {SIGQUIT, "SIGQUIT"},
    {SIGILL, "SIGILL"},
    {SIGTRAP, "SIGTRAP"},
    {SIGABRT, "SIGABRT"},
    {SIGBUS, "SIGBUS"},
    {SIGFPE, "SIGFPE"},
    {SIGKILL, "SIGKILL"},
    {SIGUSR1, "SIGUSR1"},
    {SIGSEGV, "SIGSEGV"},
    {SIGUSR2, "SIGUSR2"},
    {SIGPIPE, "SIGPIPE"},
    {SIGALRM, "SIGALRM"},
    {SIGTERM, "SIGTERM"},
    {SIGCHLD, "SIGCHLD"},
    {SIGCONT, "SIGCONT"},
    {SIGSTOP, "SIGSTOP"},
    {SIGTSTP, "SIGTSTP"},
    {SIGTTIN, "SIGTTIN"},
    {SIGTTOU, "SIGTTOU"},
    {SIGURG, "SIGURG"},
    {SIGXCPU, "SIGXCPU"},
    {SIGXFSZ, "SIGXFSZ"},
    {SIGVTALRM, "SIGVTALRM"},
    {SIGPROF, "SIGPROF"},
    {SIGWINCH, "SIGWINCH"},
    {SIGIO, "SIGIO"},
    {SIGPWR, "SIGPWR"},
    {SIGSYS, "SIGSYS"},
};

const char* signame( int signum ) {
    for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {
        const Signal& s = signals[i];
        if (s.number == signum)
            return s.name;
    }
    return "Unknown signal";
}

// Don't leak 
void signal_callback_handler(int signum)
{
	std::cout << BRed;
	std::cout << "\r[Server] Caught signal " << signum << " : " << signame(signum) << std::endl;
	std::cout << Color_Off;
	Server::destroy_instance();
	exit(signum);
}

void Server::signal_handler()
{
	signal(SIGINT, signal_callback_handler);
	signal(SIGTERM, signal_callback_handler);
	signal(SIGQUIT, signal_callback_handler);
	signal(SIGTSTP, signal_callback_handler);
}