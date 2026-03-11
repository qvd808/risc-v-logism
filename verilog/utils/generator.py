import yaml
from jinja2 import Environment, FileSystemLoader
from datetime import datetime
from collections import defaultdict

instructions_files = ["r32i_base_instruction.yaml"]
template_files     = ["opcode_defines_template.sv.j2"]
output_files       = ["opcode_defines.sv"]

TYPE_TO_FORMAT = {
    "R_type": "FORMAT_R",
    "I_type": "FORMAT_I",
    "S_type": "FORMAT_S",
    "B_type": "FORMAT_B",
    "J_type": "FORMAT_J",
    "U_type": "FORMAT_U",
}

OPCODE_TO_NAME = {
    "0000011": "LOAD",
    "0000111": "LOAD_FP",
    "0001111": "MISC_MEM",
    "0010011": "OP_IMM",
    "0010111": "AUIPC",
    "0011011": "OP_IMM_32",
    "0100011": "STORE",
    "0100111": "STORE_FP",
    "0101111": "AMO",
    "0110011": "OP",
    "0110111": "LUI",
    "0111011": "OP_32",
    "1100011": "BRANCH",
    "1100111": "JALR",
    "1101111": "JAL",
    "1110011": "SYSTEM",
}


def parse_instructions(data: dict) -> list[dict]:
    """Flatten YAML into a list of instruction dicts, sorted by opcode."""
    instructions = []

    for type_name, type_data in data["types"].items():
        for instr_name, instr_data in type_data["instructions"].items():
            encoding = instr_data["encoding"]
            instructions.append({
                "name":        instr_name,
                "type":        type_name,
                "format":      TYPE_TO_FORMAT.get(type_name, "FORMAT_I"),
                "description": instr_data.get("operation", ""),
                "encoding":    encoding,
                "has_funct3":  "funct3" in encoding,
                "has_funct7":  "funct7" in encoding,
            })

    return sorted(instructions, key=lambda i: i["encoding"]["opcode"])


def build_opcode_enums(instructions: list[dict]) -> list[dict]:
    """Group instructions by opcode value, sorted by binary value."""
    opcode_groups = defaultdict(list)

    for instr in instructions:
        opcode = instr["encoding"]["opcode"]
        opcode_groups[opcode].append(instr["name"])

    enums = []
    for opcode in sorted(opcode_groups.keys()):
        enums.append({
            "name":         OPCODE_TO_NAME.get(opcode, f"OPCODE_{opcode}"),
            "value":        opcode,
            "instructions": opcode_groups[opcode],
        })

    return enums


def build_funct3_enums(instructions: list[dict]) -> list[dict]:
    """Group instructions by funct3, labelled by common operation name."""
    funct3_groups = defaultdict(list)

    for instr in instructions:
        if "funct3" in instr["encoding"]:
            funct3_groups[instr["encoding"]["funct3"]].append(instr["name"])

    enums = []
    for funct3 in sorted(funct3_groups.keys()):
        names = funct3_groups[funct3]
        # Use first two instruction names as the enum label
        label = "_".join(n for n in names[:2])
        enums.append({
            "name":         f"FUNCT3_{label}",
            "value":        funct3,
            "instructions": names,
        })

    return enums


def build_format_case_entries(opcode_enums: list[dict], instructions: list[dict]) -> list[dict]:
    """Map each opcode enum to a FORMAT_x for the case statement."""
    opcode_to_format = {}

    for instr in instructions:
        opcode = instr["encoding"]["opcode"]
        opcode_name = OPCODE_TO_NAME.get(opcode, f"OPCODE_{opcode}")
        opcode_to_format[opcode_name] = instr["format"]

    return [
        {"opcode_name": name, "format": fmt}
        for name, fmt in opcode_to_format.items()
    ]


def main():
    with open(instructions_files[0], "r") as f:
        data = yaml.safe_load(f)

    instructions   = parse_instructions(data)
    opcode_enums   = build_opcode_enums(instructions)
    funct3_enums   = build_funct3_enums(instructions)
    format_cases   = build_format_case_entries(opcode_enums, instructions)

    env = Environment(
        loader=FileSystemLoader("."),
        trim_blocks=True,    # removes newline after {% block %}
        lstrip_blocks=True,  # strips leading spaces before {% block %}
    )
    template = env.get_template(template_files[0])

    output = template.render(
        instruction_set = data["instruction_set"],
        instructions    = instructions,
        opcode_enums    = opcode_enums,
        funct3_enums    = funct3_enums,
        format_cases    = format_cases,
        now             = datetime.now,
        source_file     = instructions_files[0],
        generator_version = "1.0",
    )

    for file in output_files:
        with open(file, "w") as f:
            f.write(output)

    print(f"✓ Generated {output_files[0]} ({len(instructions)} instructions)")


if __name__ == "__main__":
    main()
