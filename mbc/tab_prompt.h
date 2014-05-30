/// @file prompt.h
/// @brief prompt the user with tab-completion
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-30

#ifndef PROMPT_H
#define PROMPT_H

#include <readline/readline.h>
#include <readline/history.h>
#include <string>

std::string tab_prompt (const std::string &p)
{
    // configure tab key to auto-complete
    rl_bind_key('\t', rl_complete);
    // prompt the user
    char *input = readline (p.c_str ());
    // result
    std::string r;
    // check for EOF
    if (!input)
        return r;
    // add input to history
    add_history (input);
    // set result
    r = std::string (input);
    // free resources
    free (input);
    return r;
}

#endif
