//#define ENABLE_CSCOMPILER_LOG

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.CodeDom.Compiler;
using System.Reflection;
using System.IO;
using Microsoft.CSharp;

namespace BHScriptCompiler
{
    public class CSharpRTCompiler
    {
    // Memeber functions
        // Add source code to compile
        public void AddCodeToCompile( string code )
        {
            //Console.WriteLine( code );
            mSources.Add( new CodeSnippetCompileUnit( code ) );
        }

        // Get type with given name
        public CSharpType GetType( string typeName )
        {
           if ( !mCSharpTypes.ContainsKey( typeName ) )
                return null;

            return mCSharpTypes[typeName];
        }

        // Get all types
        public List<string> GetAllTypes()
        {
            List<string> result = new List<string>();
            foreach( var entry in mCSharpTypes )
                result.Add( entry.Key );

            return result;
        }

        // Compile all code
        public void CompileAllCode()
        {
#if ENABLE_CSCOMPILER_LOG
            Console.WriteLine("--------------------------------");
            Console.WriteLine("Compiling...");
#endif
            // @cleanup: Need a set directory function
            string[] engineScripts = Directory.GetFiles( "../scripts/Engine" );
            foreach( string fileName in engineScripts )
            {
#if ENABLE_CSCOMPILER_LOG
                Console.WriteLine( "Adding Engine Script: {0}", fileName );
#endif
                string code = System.IO.File.ReadAllText( fileName );
                AddCodeToCompile( code );
            }

            // Compile version = .Net Framework version
            Dictionary<string, string> cpOptions = new Dictionary<string, string>();
            cpOptions.Add( "CompilerVersion", "v4.0" );

            // Compiling C# script
            CSharpCodeProvider provider = new CSharpCodeProvider( cpOptions );

#if ENABLE_CSCOMPILER_LOG
            Console.WriteLine("Adding references...");
#endif
            // Dlls that code may depend on
            CompilerParameters cp = CreateCompilerParameters();

            BH.CPlusPlusInterface.StartProfiler( "C#Compiler" );
            // Compile into an in memory assembly
            CompilerResults cr = provider.CompileAssemblyFromDom( cp, mSources.ToArray() );
            BH.CPlusPlusInterface.StopProfiler( "C#Compiler" );

            foreach ( string output in cr.Output )
            {
                Console.WriteLine( output );
            }
            Console.WriteLine();

            // Print error
            if ( cr.Errors.Count > 0 )
            {
                foreach ( CompilerError error in cr.Errors )
                {
                    Console.WriteLine( "Compile Error: File = " + error.FileName + ",Line = " + error.Line + ", Error = " + error.ErrorText );
                    Console.WriteLine();
                }
            }
            else
            {
                Console.WriteLine( "Compile Successfully!" );
            }

            // Get all C# types
            Assembly assembly = cr.CompiledAssembly;

            // Extract the method containing the script using reflection
            Module[] modules = assembly.GetModules( false );
            Type[] types = modules[0].GetTypes();

            for ( int i = 0; i < types.Length; ++i )
            {
                Type type = types[i];
                if ( type.IsClass )  // We are find class (Object Component)
                {
                    mCSharpTypes.Add( type.Name, new CSharpType( type ) );
                }
            }

#if ENABLE_CSCOMPILER_LOG
            Console.WriteLine("--------------------------------");
#endif
        }

        // Compiler configuration for compiling
        private CompilerParameters CreateCompilerParameters()
        {
            CompilerParameters cp = new CompilerParameters();
            cp.CompilerOptions = "/target:library";
            cp.IncludeDebugInformation = true;
            cp.GenerateExecutable = false;
            cp.GenerateInMemory = true;

            // Add assembly references. (dlls)
            for ( int i = 0; i < mAssemblyReference.Count<string>(); ++i )
            {
                cp.ReferencedAssemblies.Add( mAssemblyReference[i] );
            }

            return cp;
        }

        // Add default reference
        private void AddDefaultAssemblyReference()
        {
            mAssemblyReference.Add( "mscorlib.dll" );
            mAssemblyReference.Add( "System.dll" );
            mAssemblyReference.Add( "System.Core.dll" );
            mAssemblyReference.Add( "System.Drawing.dll" );
            mAssemblyReference.Add( "System.Data.dll" );
            mAssemblyReference.Add( "System.Xml.dll" );
            mAssemblyReference.Add( "System.Xml.Linq.dll" );
            mAssemblyReference.Add( "System.Windows.Forms.dll" );
            mAssemblyReference.Add( "Microsoft.CSharp.dll" );
        }

        public object CreateObject( string type, string name )
        {
            if( !mCSharpTypes.ContainsKey( type ) )
            {
                Console.WriteLine( "{0} type not existed!", type );
                return null;
            }

            if( mVariables.ContainsKey( name ) )
            {
                Console.WriteLine( "{0} name existed!", name );
                return null;
            }

            object obj = mCSharpTypes[type].Create();
		    mVariables.Add( name, obj );
            mVariablesType.Add( name, type );
		    return obj;
        }

        public object GetObject( string name )
        {
            if( !mVariables.ContainsKey( name ) )
                return null;

            return mVariables[name];
        }

        public void DestroyObject( string name )
        {
            if( !mVariables.ContainsKey( name ) )
                return;

		    mVariables.Remove( name );
            mVariablesType.Remove( name );
        }

        public bool ScriptTypeExist( string scriptType )
        {
            return mCSharpTypes.ContainsKey( scriptType );
        }

        public string[] GetAllScriptType()
        {
            return mCSharpTypes.Keys.ToArray();
        }

        public string[] GetAllEngineScriptType()
        {
            string[] engineTypes = {
                                       "BHComponent",
                                       "BHObject",
                                       "CPlusPlusInterface",
                                       "BHVector2f",
                                       "BHVector3f",
                                       "BHVector4f",
                                       "BHQuaternion",
                                       "BHMaterial",
                                       "BHModel",
                                       "BHTexture",
                                       "BHEventManager",
                                       "KeyDelegate",
                                       "MouseDelegate",
                                       "OnCollision",
                                       "StationaryOnCollision"
                                   };

            return engineTypes;
        }

        public void ClearAllSources()
        {
            mSources.Clear();
        }

        public void RecompileAllCode()
        {
            // Destroy object
            string [] variables = mVariables.Keys.ToArray();
            int size = variables.Length;
            for ( int i = 0; i < size; ++i )
            {
                //mVariables.Remove( variables[i] );
                DestroyObject( variables[i] );
            }

            mCSharpTypes.Clear();
            CompileAllCode();

            //// Recreate object
            //for ( int i = 0; i < size; ++i )
            //{
            //    string name = variables[i];
            //
            //    // No variable name found
            //    if( !mVariablesType.ContainsKey( name ) )
            //        continue;
            //
            //    string type = mVariablesType[name];
            //
            //    // Type not found
            //    if( !mCSharpTypes.ContainsKey( type ) )
            //        continue;
            //
            //    // Variables already created
            //    if( mVariables.ContainsKey( name ) )
            //        continue;
            //
            //    object obj = mCSharpTypes[type].Create();
		    //    mVariables.Add( name, obj );
            //}
        }

    // Member Variables
        static private List<string> mAssemblyReference = new List<string>();                               //!< References
        static private List<CodeCompileUnit> mSources = new List<CodeCompileUnit>();                       //!< Compile unit
        static private Dictionary<string, CSharpType> mCSharpTypes = new Dictionary<string, CSharpType>(); //!< All scripts types
        static private Dictionary<string, object> mVariables = new Dictionary<string, object>();           //!< All variables
        static private Dictionary<string, string> mVariablesType = new Dictionary<string, string>();       //!< All variables type
    }
}
