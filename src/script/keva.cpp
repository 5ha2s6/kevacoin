// Copyright (c) 2014-2017 Daniel Kraft
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <script/keva.h>

#include <uint256.h>

std::string CKevaScript::KEVA_DISPLAY_NAME_KEY = "_KEVA_NS_";

CKevaScript::CKevaScript (const CScript& script)
  : op(OP_NOP), address(script)
{
  opcodetype nameOp;
  CScript::const_iterator pc = script.begin();
  if (!script.GetOp(pc, nameOp)) {
    return;
  }

  opcodetype opcode;
  while (true) {
    valtype vch;

    if (!script.GetOp(pc, opcode, vch)) {
      return;
    }
    if (opcode == OP_DROP || opcode == OP_2DROP || opcode == OP_NOP) {
      break;
    }
    if (!(opcode >= 0 && opcode <= OP_PUSHDATA4)) {
      return;
    }

    args.push_back (vch);
  }

  // Move the pc to after any DROP or NOP.
  while (opcode == OP_DROP || opcode == OP_2DROP || opcode == OP_NOP) {
    if (!script.GetOp(pc, opcode)) {
      break;
    }
  }
  pc--;

  /* Now, we have the args and the operation.  Check if we have indeed
     a valid name operation and valid argument counts.  Only now set the
     op and address members, if everything is valid.  */
  switch (nameOp) {
    case OP_KEVA_PUT:
      if (args.size() != 3) {
        return;
      }
      break;

    case OP_KEVA_NAMESPACE:
      if (args.size() != 3) {
        return;
      }
      break;

    default:
      return;
  }

  op = nameOp;
  address = CScript(pc, script.end());
}

CScript
CKevaScript::buildKevaPut(const CScript& addr, const valtype& nameSpace,
                                   const valtype& key, const valtype& value)
{
  CScript prefix;
  prefix << OP_KEVA_PUT << nameSpace << key << value << OP_2DROP << OP_DROP;

  return prefix + addr;
}

CScript CKevaScript::buildKevaNamespace(const CScript& addr, const uint160& nameSpace,
                                const valtype& displayName)
{
  CScript prefix;
  prefix << OP_KEVA_NAMESPACE << ToByteVector(nameSpace) << displayName << OP_2DROP;

  return prefix + addr;
}
