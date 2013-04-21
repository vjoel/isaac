# ISAAC #

A pseudorandom number generator based on http://burtleburtle.net/bob/rand/isaac.html.

Sometimes useful for simulation in place of MT19937 (which ruby's #rand is based on), even though ISAAC's number distribution quality is not as good, because it is faster and the state is lighter, so you can have many of them.

This is useful when you need multiple pseudo-random sequences that are _independent_ and _recreatable_.
