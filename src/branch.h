#pragma once

#include "globals.h"
#include "exceptions.h"
#include "commit.h"

void create_branch(string branch_name);

bool is_branch(string branch_name);

void list_branches();