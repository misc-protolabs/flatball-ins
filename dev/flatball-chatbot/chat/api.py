import os
import json
import uuid
from datetime import datetime
from fastapi import FastAPI, Request, BackgroundTasks
from fastapi.staticfiles import StaticFiles
from fastapi.responses import HTMLResponse
from pydantic import BaseModel
import uvicorn
import signal

from langchain_ollama.llms import OllamaLLM
from langchain_core.prompts import PromptTemplate
from langchain_core.runnables import RunnablePassthrough
from langchain_core.output_parsers import StrOutputParser
from langchain_chroma.vectorstores import Chroma
from langchain_ollama.embeddings import OllamaEmbeddings

# --- Configuration ---
CHROMA_PATH = "../.chroma_db"
EMBEDDING_MODEL = "nomic-embed-text"
GENERATION_MODEL = "llama3:8b"
QUERY_LOG_PATH = "../.log/queries"
SHUTDOWN_FLAG = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".shutdown.flag"))

os.makedirs(QUERY_LOG_PATH, exist_ok=True)

def shutdown_server_gracefully():
    print("Executing graceful shutdown (processes will be terminated by launcher script)...")

app = FastAPI()

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

    chain = (
        {"context": retriever, "question": RunnablePassthrough()}
        | prompt
        | ollama_llm
        | StrOutputParser()
    )
    return chain, retriever

rag_chain, retriever_instance = get_rag_chain()

class QueryRequest(BaseModel):
    question: str

class Source(BaseModel):
    source: str
    page: int

class QueryResponse(BaseModel):
    answer: str
    sources: list[Source]

def save_query_response(question, retrieved_docs, answer, start_time, end_time):
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

@app.post("/shutdown")
async def shutdown_endpoint(background_tasks: BackgroundTasks):
    print("Shutdown signal received. Scheduling graceful shutdown.")
    background_tasks.add_task(shutdown_server_gracefully)
    try:
        open(SHUTDOWN_FLAG, "w").close()
        print(f"Shutdown flag written to {SHUTDOWN_FLAG}")
    except Exception as e:
        print(f"Failed to write shutdown flag: {e}")
    return {"message": "Shutdown initiated."}

@app.post("/query", response_model=QueryResponse)
async def query_endpoint(query: QueryRequest, background_tasks: BackgroundTasks):
    question = query.question.strip()

    if question.startswith(':'):
        command = question.lower()
        if command == ':exit':
            background_tasks.add_task(shutdown_server_gracefully)
            try:
                open(SHUTDOWN_FLAG, "w").close()
                print(f"Shutdown flag written to {SHUTDOWN_FLAG}")
            except Exception as e:
                print(f"Failed to write shutdown flag: {e}")
            return QueryResponse(answer="Shutdown initiated.", sources=[])
        else:
            return QueryResponse(answer="Unrecognized command.", sources=[])

    start_time = datetime.now()
    question = query.question

    retrieved_docs = retriever_instance.invoke(question)
    sources = []
    for doc in retrieved_docs:
        sources.append(Source(
            source=os.path.basename(doc.metadata.get("source", "Unknown")),
            page=doc.metadata.get("page", 0)
        ))

    answer = rag_chain.invoke(question)
    end_time = datetime.now()

    save_query_response(question, retrieved_docs, answer, start_time, end_time)

    return QueryResponse(answer=answer, sources=sources)

app.mount("/", StaticFiles(directory="frontend", html=True), name="static")

if __name__ == "__main__":
    print("Starting FastAPI server...")
    print("Access the UI at http://127.0.0.1:8000")
    uvicorn.run(app, host="127.0.0.1", port=8000)