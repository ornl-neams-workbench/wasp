# Filler

## Overview

The **Filler** package provides a compact grammar and interpreter for structured array population.  
It enables concise specification of numeric sequences using a domain-specific syntax that is lexed,
parsed, and executed within the WASP framework.

Filler follows the standard WASP model:
- lexical analysis (Flex)
- parsing (Bison)
- execution via an interpreter over a token stream

The resulting sequence is constructed procedurally without requiring explicit enumeration.

---

## Purpose

Filler is intended for:
- compact specification of large numeric arrays
- reduction of repetitive input
- support of legacy-style encoded data formats
- preprocessing of structured numeric inputs for analysis workflows

---

## Components

The primary components of the Filler package are:

- **FillerLexerImpl**  
  Performs lexical analysis of the input stream.

- **FillerParser**  
  Defines and enforces the Filler grammar.

- **FillerInterpreter**  
  Coordinates lexing and parsing, and executes the grammar to populate the output sequence.

---

## Grammar Summary

Filler defines a positional, stateful grammar operating on a current write index.

### Fill Operations

- `iRj`, `i*j`, `i$j`  
  Store value `j` in the next `i` positions.

- `iPj`  
  Alternating sequence: `j, -j, j, ...` for `i` entries.

- `Fj`  
  Fill remaining positions with value `j`.

- `iz`  
  Insert `i` zeros.

---

### Position Control

- `Aj`  
  Set the current position to index `j`.  
  The next value is written at `j + 1`.

- `iS`  
  Shift the current position by `i` (positive or negative).

---

### Direct Values

- `integer`, `double`  
  Store value at the current position.

---

### Repetition

- `i?Qj`  
  Repeat the previous `j` entries `i` times (default `i = 1`).

- `i?Nj`  
  Repeat the previous `j` entries `i` times, inverting each repetition.

- `i?Bj`  
  Move back `i` positions, then repeat the previous `j` entries in reverse order.

---

### Interpolation

- `iIj k`  
  Linear interpolation between `j` and `k` with `i` intermediate points  
  (total `i + 2` values). Requires whitespace separation.

- `iLj k`  
  Logarithmic interpolation between `j` and `k` with `i` intermediate points.

---

## Notes

- The grammar is **stateful**; many operations depend on previously generated values.
- Positioning operations (`A`, `S`) affect all subsequent writes.
- Interpolation requires valid numeric spacing and ordering.
- Integer interpolation requires `(k - j) / (i + 1)` to be integral.

---

## Examples

### Basic Fill

```text
5R1
```

Produces:

```text
[1, 1, 1, 1, 1]
```

```text
4P2
```

Produces:

```text
[2, -2, 2, -2]
```

```text
3z
```

Produces:

```text
[0, 0, 0]
```

### Addressing (Ai)

```text
5R1 A2 3R9
```

Produces:

```text
[1, 9, 9, 9]
```

```text
A1 3R7
```

Produces:

```text
[7, 7, 7]
```

### Shift (iS)

```text
3R1 2S 2R5
```

Produces:

```text
[1, 1, 1, ?, ?, 5, 5]
```

```text
4R2 -2S 2R9
```

Produces:

```text
[2, 2, 9, 9]
```

### Repetition (iQj, iNj, iBj)

```text
1 2 3 2Q3
```

Produces:

```text
[1, 2, 3, 1, 2, 3, 1, 2, 3]
```

```text
1 2 2N2
```

Produces:

```text
[1, 2, -1, -2, 1, 2]
```

```text
1 2 3 1B2
```

Produces:

```text
[1, 2, 3, 2, 1]
```

### Interpolation (iIjk, iLjk)

```text
2I0 6
```

Produces:

```text
[0, 2, 4, 6]
```

```text
2L1 100
```

Produces:

```text
[1, 10, 100]
```

### Mixed

```text
F0 3R1 2P2 A3 2R8 2S 2I0 4
```

Produces:

```text
[1, 1, 8, 8, -2, 0, 1, 34, 67, 100]
```