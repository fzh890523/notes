## **IB: Implementation-defined behaviour** 

The standard leaves it up to the particular compiler/platform to define the precise behaviour, but requires that it be defined.

Using implementation-defined behaviour can be useful, but makes your code less portable.

## **UB: Undefined behaviour**

The standard does not specify how a program invoking undefined behaviour should behave. Also known as "nasal demons" because theoretically it could make demons fly out of your nose.

Using undefined behaviour is nearly always a bad idea. Even if it seems to work sometimes, any change to environment, compiler or platform can randomly break your code.



