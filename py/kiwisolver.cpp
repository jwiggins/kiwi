/*-----------------------------------------------------------------------------
| Copyright (c) 2013, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file COPYING.txt, distributed with this software.
|----------------------------------------------------------------------------*/
#include <Python.h>
#include <kiwi/kiwi.h>
#include "pythonhelpers.h"
#include "py23compat.h"
#include "types.h"

#define PY_KIWI_VERSION "0.1.3"

using namespace PythonHelpers;

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#define INITERROR return mod
#define FROM_STRING PyUnicode_FromString
#else
#define GETSTATE(m) (&_kiwisolverstate)
static struct module_state _kiwisolverstate;
#define INITERROR return
#define FROM_STRING PyString_FromString
#endif

static PyMethodDef
kiwisolver_methods[] = {
    { 0 } // Sentinel
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef kiwisolver_moduledef = {
    PyModuleDef_HEAD_INIT,
    "kiwisolver",
    NULL,
    sizeof( struct module_state ),
    kiwisolver_methods,
    NULL
};

PyMODINIT_FUNC
PyInit_kiwisolver( void )
#else
PyMODINIT_FUNC
initkiwisolver( void )
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *mod = PyModule_Create( &kiwisolver_moduledef );
#else
    PyObject* mod = Py_InitModule( "kiwisolver", kiwisolver_methods );
#endif
    if( !mod )
        INITERROR;
    if( import_variable() < 0 )
        INITERROR;
    if( import_term() < 0 )
        INITERROR;
    if( import_expression() < 0 )
        INITERROR;
    if( import_constraint() < 0 )
        INITERROR;
    if( import_solver() < 0 )
        INITERROR;
    if( import_strength() < 0 )
        INITERROR;
    PyObject* kiwiversion = Py23Str_FromString( KIWI_VERSION );
    if( !kiwiversion )
        INITERROR;
    PyObject* pyversion = Py23Str_FromString( PY_KIWI_VERSION );
    if( !pyversion )
        INITERROR;
    PyObject* pystrength = PyType_GenericNew( &strength_Type, 0, 0 );
    if( !pystrength )
        INITERROR;

    PyModule_AddObject( mod, "__version__", pyversion );
    PyModule_AddObject( mod, "__kiwi_version__", kiwiversion );
    PyModule_AddObject( mod, "strength", pystrength );
    PyModule_AddObject( mod, "Variable", newref( pyobject_cast( &Variable_Type ) ) );
    PyModule_AddObject( mod, "Term", newref( pyobject_cast( &Term_Type ) ) );
    PyModule_AddObject( mod, "Expression", newref( pyobject_cast( &Expression_Type ) ) );
    PyModule_AddObject( mod, "Constraint", newref( pyobject_cast( &Constraint_Type ) ) );
    PyModule_AddObject( mod, "Solver", newref( pyobject_cast( &Solver_Type ) ) );
    PyModule_AddObject( mod, "DuplicateConstraint", newref( DuplicateConstraint ) );
    PyModule_AddObject( mod, "UnsatisfiableConstraint", newref( UnsatisfiableConstraint ) );
    PyModule_AddObject( mod, "UnknownConstraint", newref( UnknownConstraint ) );
    PyModule_AddObject( mod, "DuplicateEditVariable", newref( DuplicateEditVariable ) );
    PyModule_AddObject( mod, "UnknownEditVariable", newref( UnknownEditVariable ) );
    PyModule_AddObject( mod, "BadRequiredStrength", newref( BadRequiredStrength ) );

#if PY_MAJOR_VERSION >= 3
    return mod;
#endif
}
