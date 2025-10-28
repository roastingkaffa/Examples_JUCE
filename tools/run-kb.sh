#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "$ROOT"

echo "==> Step 1/2: 產生/更新 Knowledge Base（/fkb）"
claude /fkb "Scan firmware-kb-create/**, Drivers/**, Middlewares/**, Source/**, src/** and update module docs in project-kb/03-software-modules. Keep existing front-matter. Summarize changes at the end."

echo
echo "==> Step 2/2: 驗證 KB 完整性（/agents/doc-validator）"
claude /agents/doc-validator "Validate KB and list missing metadata, broken links, and a prioritized action list."

