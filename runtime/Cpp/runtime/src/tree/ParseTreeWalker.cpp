/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

#include "tree/ErrorNode.h"
#include "ParserRuleContext.h"
#include "tree/ParseTreeListener.h"
#include "support/CPPUtils.h"

#include "tree/IterativeParseTreeWalker.h"
#include "tree/ParseTreeWalker.h"

using namespace antlr4::tree;
using namespace antlrcpp;

static IterativeParseTreeWalker defaultWalker;
ParseTreeWalker &ParseTreeWalker::DEFAULT = defaultWalker;

ParseTreeWalker::~ParseTreeWalker() {
}

void ParseTreeWalker::walk(ParseTreeListener *listener, ParseTree *t) const {
  ErrorNode *const errorNode = antlr_cast<ErrorNode *>(t);
  if (errorNode != nullptr) {
    listener->visitErrorNode(errorNode);
    return;
  }
  else {
    TerminalNode *const terminalNode = antlr_cast<TerminalNode *>(t);
    if (terminalNode != nullptr) {
      listener->visitTerminal(terminalNode);
      return;
    }
  }

  enterRule(listener, t);
  for (auto &child : t->children) {
    walk(listener, child);
  }
  exitRule(listener, t);
}

void ParseTreeWalker::enterRule(ParseTreeListener *listener, ParseTree *r) const {
  ParserRuleContext *ctx = antlr_cast<ParserRuleContext *>(r);
  listener->enterEveryRule(ctx);
  ctx->enterRule(listener);
}

void ParseTreeWalker::exitRule(ParseTreeListener *listener, ParseTree *r) const {
  ParserRuleContext *ctx = antlr_cast<ParserRuleContext *>(r);
  ctx->exitRule(listener);
  listener->exitEveryRule(ctx);
}
