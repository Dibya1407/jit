#pragma once

#include <stdexcept>

class JitError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidArgument : public JitError {
public:
    using JitError::JitError;
};

class RepoNotFound : public JitError {
public:
    using JitError::JitError;
};

class ObjectNotFound : public JitError {
public:
    using JitError::JitError;
};

class InvalidObject : public JitError {
public:
    using JitError::JitError;
};

class InvalidTree : public JitError {
public:
    using JitError::JitError;
};

class InvalidCommit : public JitError {
public:
    using JitError::JitError;
};

class ConfigError : public JitError {
public:
    using JitError::JitError;
};