# Mathy
A formula evaluator

## What is this
This is a simple computer algebra system which I am developing.
It is currently able to evaluate simple mathematical expressions and
even calculate derivatives of functions.

It's weakness at the moment is, that it still can't really simplify
terms. The input
<tt>d(x^2, x)</tt>
will calculate the derivative of x^2, which should equal 2\*x.
It outputs
<tt>x ^ 2 * (2 / x + 0)</tt>
which is certainly not wrong, but just not really beautifully written.

