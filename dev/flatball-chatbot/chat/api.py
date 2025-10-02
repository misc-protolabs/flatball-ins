# Copyright 2025 Michael V. Schaefer
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import json
import uuid
from datetime import datetime
from fastapi import FastAPI, Request
from fastapi.staticfiles import StaticFiles
from fastapi.responses import HTMLResponse
from pydantic import BaseModel
import uvicorn

from langchain_ollama.llms import OllamaLLM
from langchain_core.prompts import PromptTemplate
from langchain_core.runnables import RunnablePassthrough
from langchain_core.output_parsers import StrOutputParser
from langchain_chroma.vectorstores import Chroma
from langchain_ollama.embeddings import OllamaEmbeddings

# --- Configuration ---
CHROMA_PATH = "./.chroma_db"
EMBEDDING_MODEL = "nomic-embed-text"
GENERATION_MODEL = "llama3:8b"
QUERY_LOG_PATH = "../.log/queries" # Centralized log path

# Ensure query log directory exists
os.makedirs(QUERY_LOG_PATH, exist_ok=True)

# --- FastAPI App ---
app = FastAPI()

# --- RAG Chain Initialization ---
def get_rag_chain():
    embeddings = OllamaEmbeddings(model=EMBEDDING_MODEL)
    vector_store = Chroma(persist_directory=CHROMA_PATH, embedding_function=embeddings)
    retriever = vector_store.as_retriever()

    template = """Answer the question based only on the following context. If you don't know the answer, just say that you don't know. Don't try to make up an answer.

Context:
{context}

Question: {question}
"""
    prompt = PromptTemplate.from_template(template)
    ollama_llm = OllamaLLM(model=GENERATION_MODEL, num_predict=1024)

    # The chain for getting the answer
    chain = (
        {"context": retriever, "question": RunnablePassthrough()}
        | prompt
        | ollama_llm
        | StrOutputParser()
    )
    return chain, retriever

rag_chain, retriever_instance = get_rag_chain()

# --- API Models ---
class QueryRequest(BaseModel):
    question: str

class Source(BaseModel):
    source: str
    page: int

class QueryResponse(BaseModel):
    answer: str
    sources: list[Source]

# --- Query Logging Function ---
def save_query_response(question, retrieved_docs, answer, start_time, end_time):
    """Saves the question, sources, and answer to a UUID-named markdown file."""
    file_id = str(uuid.uuid4())
    filename = os.path.join(QUERY_LOG_PATH, f"{file_id}.md")
    duration = end_time - start_time

    sources_text = "\n".join([
        f"- **Source:** {os.path.basename(doc.metadata.get("source", "Unknown"))}, **Page:** {doc.metadata.get("page", "N/A")}"
        for doc in retrieved_docs
    ])

    content = f"""# Chatbot Query Metrics

**Start Time:** {start_time.strftime("%Y-%m-%d %H:%M:%S")}
**End Time:** {end_time.strftime("%Y-%m-%d %H:%M:%S")}
**Duration:** {duration.total_seconds():.2f} seconds

---

## ❓ Question

{question}

---

## 💡 Answer

{answer}

---

## 📚 Sources Used

{sources_text}
"""

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f"Answer saved to {filename}")

# --- API Endpoints ---
@app.post("/query", response_model=QueryResponse)
async def query_endpoint(query: QueryRequest):
    """Endpoint to ask a question to the RAG chatbot."""
    start_time = datetime.now()
    question = query.question
    
    # Get sources first
    retrieved_docs = retriever_instance.invoke(question)
    sources = []
    for doc in retrieved_docs:
        sources.append(Source(
            source=os.path.basename(doc.metadata.get("source", "Unknown")),
            page=doc.metadata.get("page", 0) # Default to page 0 if not available
        ))

    # Get the answer
    answer = rag_chain.invoke(question)
    
    end_time = datetime.now()
    
    # Save the query and response to log
    save_query_response(question, retrieved_docs, answer, start_time, end_time)

    return QueryResponse(answer=answer, sources=sources)

# --- Static Files ---
# This serves the frontend directory and must be mounted after all other API routes
app.mount("/", StaticFiles(directory="frontend", html=True), name="static")


if __name__ == "__main__":
    print("Starting FastAPI server...")
    print("Access the UI at http://127.0.0.1:8000")
    uvicorn.run(app, host="127.0.0.1", port=8000)
