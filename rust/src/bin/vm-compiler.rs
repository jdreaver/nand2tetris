use std::env;
use std::fs;
use std::path::Path;
use std::process;

use nand2tetris::misc::{SourceLine, Symbol};
use nand2tetris::vm;

fn main() {
    if env::args().len() <= 1 {
        eprintln!("Usage: assembler <file1> [file2] ...");
        process::exit(1);
    }

    // TODO: The bootstrap code and footer should be in the vm module. This
    // function should just read .vm files (perhaps also parsing them), and then
    // feed them into the translator as a stream.

    // Bootstrap code, set SP = 256 and call Sys.init
    println!("@256");
    println!("D=A");
    println!("@SP");
    println!("M=D");

    let bootstrap_psuedo_filename = "_vm_bootstrap".to_string();
    let sys_init_call = SourceLine {
        lineno: 0,
        item: vm::VMCommand::Call(vm::CallCommand {
            name: Symbol("Sys.init".to_string()),
            nargs: 0,
        }),
    };
    let bootstrap_call_commands = vm::vm_command_to_asm(
        &bootstrap_psuedo_filename,
        &sys_init_call,
        &mut "".to_string(),
        &mut 0,
    )
    .expect("failed to build bootstrap Sys.init call");
    for line in bootstrap_call_commands {
        println!("{}", line);
    }

    for path_str in env::args().skip(1) {
        let path = Path::new(&path_str);
        let file_basename: &str = path
            .file_stem()
            .expect("couldn't get file basename")
            .to_str()
            .expect("couldn't convert file basename to &str");

        let source_string =
            fs::read_to_string(path.clone()).expect("Something went wrong reading the source file");

        let parsed = vm::parse_vm_source(&source_string).expect("failed to parse source");
        // println!("{:#?}", parsed);

        let asm = vm::vm_to_asm(&file_basename.to_string(), parsed)
            .expect("failed to compile source to ASM");

        println!("// SOURCE: {file_basename}");
        for line in asm {
            println!("{}", line);
        }
        println!("");
    }

    // Add footer that ends program with infinite loop
    println!("(_vm_END)");
    println!("@_vm_END");
    println!("0;JMP");
}
