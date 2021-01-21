"""
from: https://gist.github.com/huzecong/df51502a8a6ec0bcc0e605a2ce109008
"""

import collections
import typing


__all__ = [
    "Options",
]


class OptionsMeta(typing.NamedTupleMeta):
    def __new__(mcs, typename, bases, namespace):
        if namespace.get('_root', False):
            # The created class is `Options`, skip.
            return super().__new__(mcs, typename, bases, namespace)

        # Gather fields from annotations of current class and base classes.
        cur_fields = namespace.get('__annotations__', {})
        fields = {}
        field_sources = {}  # which base class does the name came from
        field_defaults = {}
        for base in bases:
            if issubclass(base, Options) and hasattr(base, '_fields'):
                # Base class is a concrete subclass of `Options`.
                for name in base._fields:
                    if name in cur_fields:
                        # Make sure not to overwrite redefined fields.
                        continue
                    if name in fields:
                        # Overlapping field that is not redefined.
                        raise TypeError(
                            f"Base class {base} contains field {name}, which "
                            f"is defined in other base class "
                            f"{field_sources[name]}")
                    fields[name] = base.__annotations__[name]
                    field_sources[name] = base
                    if name in base._field_defaults:
                        field_defaults[name] = base._field_defaults[name]
        fields.update(cur_fields)
        if len(fields) == 0:
            raise ValueError("Options class must contain at least one field")
        for name, value in field_defaults.items():
            namespace.setdefault(name, value)
        
        # Reorder fields to put those without default values in front.
        fields_with_default = [name for name in fields if name in namespace]
        reordered_fields = (sorted(set(fields).difference(fields_with_default)) +
                            sorted(fields_with_default))
        namespace['__annotations__'] = collections.OrderedDict(
            [(name, fields[name]) for name in reordered_fields])

        # Let `NamedTupleMeta` create a annotated `namedtuple` for us.
        # Note that `bases` is not used here so we just set it to `None`.
        nm_tpl = super().__new__(mcs, typename, None, namespace)

        # Rewrite `__new__` method to make all arguments keyword-only.
        # This is very hacky code. Do not try this at home.
        arg_list = ''.join(name + ', '  # watch out for singleton tuples
                           for name in reordered_fields)
        s = (f"""
        def __new__(_cls, *args, {arg_list}):
            if len(args) > 0:
                raise TypeError("Instances of Options class must be created "
                                "with keyword arguments.")
            return _tuple_new(_cls, ({arg_list}))
        """).strip()
        new_method_namespace = {'_tuple_new': tuple.__new__,
                                '__name__': f'namedtuple_{typename}'}
        exec(s, new_method_namespace)
        __new__ = new_method_namespace['__new__']
        __new__.__qualname__ = f"{typename}.__new__"
        __new__.__doc__ = nm_tpl.__new__.__doc__
        __new__.__annotations__ = nm_tpl.__new__.__annotations__
        __new__.__kwdefaults__ = {name: namespace[name]
                                  for name in fields_with_default}
        nm_tpl.__new__ = __new__

        # Wrap the return type in `OptionsMeta` so it can be subclassed.
        new_namespace = nm_tpl.__dict__.copy()
        new_namespace['_bases'] = bases
        # Also keep base classes of the `namedtuple` (i.e., the `tuple` class),
        # so we can call `tuple.__new__`.
        options_type = type.__new__(mcs, typename, nm_tpl.__bases__, new_namespace)
        options_type.__bases__ = tuple(options_type.__bases__)
        return options_type

    def mro(cls):
        default_mro = super().mro()
        # `Options` does not define `_bases`, so we don't do anything about it.
        if hasattr(cls, '_bases'):
            # `default_mro` should be `[cls, tuple, object]`.
            # `c3merge` and `c3mro` are implementations of the C3 linearization
            # algorithm, which unluckily aren't provided as APIs.
            return c3merge([
                default_mro[:1],
                *[base.__mro__ for base in cls._bases],
                default_mro[1:]])
        return default_mro


class Options(metaclass=OptionsMeta):
    _root = True

    def __new__(cls, *args, **kwargs):
        # Copied from typing.Generic.
        if cls is Options:
            # Prevent instantiation of `Options` class.
            raise TypeError("Type Options cannot be instantiated; "
                            "it can be used only as a base class")
        if (super().__new__ is object.__new__ and
                cls.__init__ is not object.__init__):
            obj = super().__new__(cls)
        else:
            obj = super().__new__(cls, *args, **kwargs)
        return obj


def c3merge(sequences):
    r"""Adapted from https://www.python.org/download/releases/2.3/mro/"""
    # Make sure we don't actually mutate anything we are getting as input.
    sequences = [list(x) for x in sequences]
    result = []
    while True:
        # Clear out blank sequences.
        sequences = [x for x in sequences if x]
        if not sequences:
            return result
        # Find the first clean head.
        for seq in sequences:
            head = seq[0]
            # If this is not a bad head (i.e., not in any other sequence)
            if not any(head in s[1:] for s in sequences):
                break
        else:
            raise Error("inconsistent hierarchy")
        # Move the head from the front of all sequences to the end of results.
        result.append(head)
        for seq in sequences:
            if seq[0] == head:
                del seq[0]
    return result
