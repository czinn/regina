/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Base64SigDecoder
static const char *Base64SigDecoder =
R"doc(A helper class for reading signatures that use base64 encodings. These
are (in particular) used in the default encodings for Regina's own
isomorphism signatures and knot signatures.

To use this class: create a new Base64SigDecoder by passing details of
the encoded string to its constructor, and then call its
``decode...()`` member functions to read values sequentially from the
encoding.

This class will keep track of a current position in the encoded
string. Each call to a ``decode...()`` member function will advance
this position accordingly (but never beyond the end of the string).

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.)doc";

// Docstring regina::python::doc::Base64SigEncoder
static const char *Base64SigEncoder =
R"doc(A helper class for writing signatures that use base64 encodings. These
are (in particular) used in the default encodings for Regina's own
isomorphism signatures and knot signatures.

To use this class: create a new Base64SigEncoder, call one or more of
its member functions to write values to the encoding, and then call
str() to extract the resulting base64 string.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.)doc";

// Docstring regina::python::doc::Base64SigEncoding
static const char *Base64SigEncoding =
R"doc(A deprecated set of helper tools for signatures that use base64
encodings. These are (in particular) used in the default encodings for
Regina's own isomorphism signatures and knot signatures.

This base64 encoding uses the characters: ``a..zA..Z0..9+-``

.. warning::
    Note that this base64 encoding uses a different set of printable
    symbols from the encoding used in utilities/base64.h. This should
    not be a problem: Regina uses this encoding exclusively for
    signatures, and uses utilities/base64.h exclusively for encoding
    files.

.. deprecated::
    This is now deprecated in favour of the new classes
    Base64SigEncoder and Base64SigDecoder, which carry state and have
    better error handling.)doc";

namespace Base64SigDecoder_ {

// Docstring regina::python::doc::Base64SigDecoder_::__init
static const char *__init =
R"doc(Creates a new decoder for the given encoded string.

Parameter ``encoding``:
    a reference to the encoded string. This string _must_ remain alive
    and unchanged for the entire lifespan of this decoder.

Parameter ``skipInitialWhitespace``:
    ``True`` if the current position should immediately advance past
    any initial whitespace in the given string.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeInt
static const char *decodeInt =
R"doc(Decodes the next non-negative integer value, assuming this uses a
fixed number of base64 characters. This integer value would typically
have been encoded using Base64SigEncoder::encodeInt(), with the same
*nChars* argument.

Specifically, it will be assumed that the integer has been broken into
*nChars* 6-bit blocks, each encoded as a single base64 character,
presented in order from lowest to highest significance.

The inverse to this routine is Base64SigEncoder::encodeInt().

Exception ``InvalidInput``:
    There are fewer than *nChars* characters available in the encoded
    string, or a character was encountered that was not a valid base64
    character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type. The result will be assembled using
    bitwise OR and bitwise shift lefts, and it is assumed that the
    programmer has chosen an integer type large enough to contain
    whatever values they expect to read.

Parameter ``nChars``:
    the number of base64 characters to read.

Returns:
    the integer that was decoded.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeSingle
static const char *decodeSingle =
R"doc(Decodes the 6-bit integer value represented by the next single base64
character.

The inverse to this routine is Base64SigEncoder::encodeSingle().

Exception ``InvalidInput``:
    There are no more characters remaining in the encoded string, or
    the next character is not a valid base64 character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeSize
static const char *decodeSize =
R"doc(Decodes the next non-negative integer value (typically representing
the size of some object), without knowing in advance how many base64
characters were used to encode it. This integer value must have been
encoded using Base64SigEncoder::encodeSize().

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also returns the smallest integer *b* with the property
that any integer *x* between 0 and *size* inclusive can be encoded
using *b* base64 characters. Typically these *x* would be _indices_
into an object (e.g., top-dimensional simplex numbers, or crossing
numbers). More precisely, *b* is the same integer that was returned
when *size* was encoded using encodeSize(). Typically you would pass
*b* to subsequent calls to decodeInt().

The inverse to this routine is Base64SigEncoder::encodeSize().

Exception ``InvalidInput``:
    There are not enough characters available in the encoded string,
    or a character was encountered that was not a valid base64
    character.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type. The result will be assembled using
    bitwise OR and bitwise shift lefts, and it is assumed that the
    programmer has chosen an integer type large enough to contain
    whatever values they expect to read.

Returns:
    a pair (*size*, *b*), where *size* is the integer that was
    decoded, and *b* is the number of base64 characters described
    above.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::decodeTrits
static const char *decodeTrits =
R"doc(Decodes three trits from a single base64 character, and returns these
as a fixed-size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is Base64SigEncoder::encodeTrits(); see
that routine for details of the encoding.

Exception ``InvalidInput``:
    There are no more characters remaining in the encoded string, or
    the next character is not a valid base64 character.

Returns:
    an array containing the three trits that were decoded.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::done
static const char *done =
R"doc(Determines whether the current position has reached the end of the
string.

Parameter ``ignoreWhitespace``:
    ``True`` if we should ignore any trailing whitespace. If there is
    whitespace at the current position, the current position will not
    be changed; this will merely make the test succeed if _only_
    whitespace characters remain.

Returns:
    ``True`` if and only if the current position is the end of the
    string.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::isValid
static const char *isValid =
R"doc(Is the given character one of the printable base64 characters
recognised by this class?

Parameter ``c``:
    the character to examine.

Returns:
    ``True`` if and only if *c* is one of the 64 printable characters
    described in the class notes.)doc";

// Docstring regina::python::doc::Base64SigDecoder_::skipWhitespace
static const char *skipWhitespace =
R"doc(Moves the current position past any whitespace.

The movement will stop upon reaching either a non-whitespace character
or the end of the string.)doc";

}

namespace Base64SigEncoder_ {

// Docstring regina::python::doc::Base64SigEncoder_::__default
static const char *__default = R"doc(Creates a new encoder, with an empty base64 string.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeInt
static const char *encodeInt =
R"doc(Encodes the given non-negative integer using a fixed number of base64
characters.

Specifically, the integer *val* will be broken into *nChars* distinct
6-bit blocks, which will be encoded in order from lowest to highest
significance.

The inverse to this routine is Base64SigDecoder::decodeInt().

Exception ``InvalidArgument``:
    The given integer *val* is negative, or requires more than ``6 ×
    nChars`` bits.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type.

Parameter ``val``:
    the non-negative integer to encode.

Parameter ``nChars``:
    the number of base64 characters to use; typically this would be
    obtained through an earlier call to encodeSize().)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeSingle
static const char *encodeSingle =
R"doc(Encodes the given 6-bit integer using a single base64 character.

The inverse to this routine is Base64SigDecoder::decodeSingle().

Exception ``InvalidArgument``:
    The given integer is not between 0 and 63 inclusive.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type.

Parameter ``c``:
    an integer between 0 and 63 inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeSize
static const char *encodeSize =
R"doc(Encodes the given non-negative integer (typically representing the
size of some object), without knowing in advance how many base64
characters will be required.

A typical use case would be where *size* represents the number of top-
dimensional simplices in a triangulation, or the number of crossings
in a link diagram.

This routine also computes the smallest integer *b* with the property
that any integer *x* between 0 and *size* inclusive can be encoded
using *b* base64 characters. In other words, any such *x* can be
encoded by calling ``encodeInt(x, b)``. Typically these *x* would be
_indices_ into an object (e.g., top-dimensional simplex numbers, or
crossing numbers). Note that encodeSize() itself might write more than
*b* characters.

The inverse to this routine is Base64SigDecoder::decodeSize().

Exception ``InvalidArgument``:
    The given integer is negative.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type.

Parameter ``size``:
    the non-negative integer to encode.

Returns:
    nChars the number of base64 characters required to write any
    integer between 0 and *size* inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::encodeTrits
static const char *encodeTrits =
R"doc(Encodes up to three trits, which are given using a fixed-size array. A
_trit_ is either 0, 1 or 2.

The given trits will be packed into a single base64 character, with
the first trit representing the lowest-significance bits of the
underlying integer and so on.

The inverse to this routine is Base64SigDecoder::decodeTrits().

Template parameter ``nTrits``:
    the number of trits to encode; this must be between 0 and 3
    inclusive.

Parameter ``trits``:
    the array of trits to encode. Each trit must take the value 0, 1
    or 2.)doc";

// Docstring regina::python::doc::Base64SigEncoder_::str
static const char *str =
R"doc(Returns the base64 encoding that has been constructed thus far.

Returns:
    The current base64 encoding.)doc";

}

namespace Base64SigEncoding_ {

// Docstring regina::python::doc::Base64SigEncoding_::decodeInt
static const char *decodeInt =
R"doc(Read the integer that is encoded at the beginning of the given base64
string.

It will be assumed that the integer is encoded using *nChars* base64
characters, each containing 6 bits of the integer, with the lowest-
significance bits encoded in the first characters.

The inverse to this routine is encodeInt().

Precondition:
    The given string contains at least *nChars* characters.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type, such as ``uint8_t``, or ``unsigned``,
    or ``long long``. The result will be assembled using bitwise OR
    and bitwise shift lefts, and it is assumed that the programmer has
    chosen an integer type large enough to contain whatever values
    they expect to read.

Parameter ``s``:
    the string from which the encoded base64 characters should be
    read.

Parameter ``nChars``:
    the number of base64 characters to read.

Returns:
    the native integer that was encoded.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::decodeSingle
static const char *decodeSingle =
R"doc(Determine the integer value represented by the given base64 character.

The inverse to this routine is encodeSingle().

Parameter ``c``:
    a base64 character, which must be one of the 64 printable
    characters described in the class notes.

Returns:
    the corresponding integer, which will be between 0 and 63
    inclusive.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::decodeTrits
static const char *decodeTrits =
R"doc(Decodes a single base64 character into three trits, and returns these
as a fixed-size array. A _trit_ is either 0, 1 or 2.

The inverse to this routine is encodeTrits(); see that routine for
details of the encoding.

Parameter ``c``:
    the base64 character to decode.

Returns:
    an array containing the three trits that had been encoded in the
    given base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::encodeInt
static const char *encodeInt =
R"doc(Append a base64 encoding of the given integer to the given string. The
integer will be broken into *nChars* distinct 6-bit blocks, and the
lowest-significance blocks will be written first.

The inverse to this routine is decodeInt().

Precondition:
    The given integer *val* is non-negative, and fits within 6*nChars*
    bits.

Python:
    The template argument *IntType* is taken to be a native C++
    ``long``.

Template parameter ``IntType``:
    a native C++ integer type, such as ``uint8_t``, or ``unsigned``,
    or ``long long``.

Parameter ``s``:
    the string that resulting characters should be appended to.

Parameter ``val``:
    the integer to encode.

Parameter ``nChars``:
    the number of base64 characters to use.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::encodeSingle
static const char *encodeSingle =
R"doc(Determine the base64 character that represents the given integer
value.

The inverse to this routine is decodeSingle().

Parameter ``c``:
    an integer between 0 and 63 inclusive.

Returns:
    the corresponding printable base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::encodeTrits
static const char *encodeTrits =
R"doc(Returns a single base64 character that encodes up to three trits,
given using a fixed-size array. A _trit_ is either 0, 1 or 2.

The given trits will be packed into a single base64 character, with
the first trit representing the lowest-significance bits of the
underlying integer and so on.

The inverse to this routine is decodeTrits().

Template parameter ``nTrits``:
    the number of trits to encode; this must be between 0 and 3
    inclusive.

Parameter ``trits``:
    the array of trits to encode. Each trit must take the value 0, 1
    or 2.

Returns:
    the resulting printable base64 character.)doc";

// Docstring regina::python::doc::Base64SigEncoding_::isValid
static const char *isValid =
R"doc(Is the given character a valid base64 character?

Returns:
    ``True`` if and only if the given character is one of the 64
    printable characters described in the class notes.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

