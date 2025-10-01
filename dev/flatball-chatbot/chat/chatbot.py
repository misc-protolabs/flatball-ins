import os
import json
import sys
import time
import shutil
import ollama
import logging
import threading
import itertools
from datetime import datetime
from langchain_community.document_loaders import PyPDFLoader, TextLoader
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_ollama.llms import OllamaLLM
from langchain_core.prompts import PromptTemplate
from langchain_core.runnables import RunnablePassthrough
from langchain_core.output_parsers import StrOutputParser
from langchain_chroma.vectorstores import Chroma # Updated import
from langchain_ollama.embeddings import OllamaEmbeddings # Updated import
from tqdm import tqdm

# --- Configuration ---
DOCUMENTS_PATH = "./.train"
PROCESSED_PATH = "./.train/processed"
CHROMA_PATH = "./.chroma_db"
EMBEDDING_MODEL = "nomic-embed-text"
LOG_FILE = "../.log/run-log.md"
QUERY_LOG_PATH = "../.log/query_logs"
METADATA_FILE = "db_metadata.json"
HEARTBEAT_SPEED_SECONDS = 0.25 # Adjust this value to speed up or slow down the animation
GENERATION_MODEL = "llama3:8b" # The model to use for generating answers (e.g., "phi3", "llama3:8b")

# --- Custom Logging Setup ---
USER_INFO_LEVEL = 25
logging.addLevelName(USER_INFO_LEVEL, "USER_INFO")
def user_info(self, message, *args, **kws):
    if self.isEnabledFor(USER_INFO_LEVEL):
        self._log(USER_INFO_LEVEL, message, args, **kws)
logging.Logger.user_info = user_info
log = logging.getLogger(__name__)

def setup_logging():
    """Sets up logging with different levels for file and console."""
    log.setLevel(logging.INFO) # The root logger will capture everything at INFO level and above.
    if log.hasHandlers():
        log.handlers.clear()

    with open(LOG_FILE, 'w', encoding='utf-8') as f:
        f.write(f"# Chatbot Run Log - {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
    
    # File handler - logs ALL messages (INFO and above)
    file_handler = logging.FileHandler(LOG_FILE, mode='a', encoding='utf-8')
    # The formatter for the file will now distinguish between INFO and USER_INFO
    file_formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(message)s")
    file_handler.setFormatter(file_formatter)
    file_handler.setLevel(logging.INFO) # Set to the lowest level we care about

    # Console handler - logs only USER_INFO and higher (warnings, errors)
    console_handler = logging.StreamHandler()
    console_formatter = logging.Formatter('%(message)s') # Keep console output clean
    console_handler.setFormatter(console_formatter)
    console_handler.setLevel(USER_INFO_LEVEL)

    log.addHandler(file_handler)
    log.addHandler(console_handler)

def main():
    """Main function to build and intelligently update the vector store."""
    # --- Setup directories ---
    os.makedirs(DOCUMENTS_PATH, exist_ok=True)
    os.makedirs(PROCESSED_PATH, exist_ok=True)
    # Ensure the log directory exists before setting up logging
    os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)

    setup_logging()
    log.user_info("==================================================")
    log.user_info("🚀 Starting Frisbee Aerodynamics Chatbot")
    log.user_info("==================================================")

    if "--clean" in sys.argv:
        clean_database()

    check_embedding_model()
    
    files_to_process, metadata = get_files_to_process()
    
    if not files_to_process:
        log.user_info("✅ Knowledge base is up-to-date.")
        query_rag()
    else:
        log.user_info(f"Found {len(files_to_process)} new or modified files to process.")
        process_files(files_to_process, metadata)

    log.user_info("\n==================================================")
    log.user_info("🎉 Process complete!")
    log.user_info("==================================================")

def query_rag():
    """Loads the vector store and enters an interactive query loop."""
    log.user_info("\n==================================================")
    log.user_info("🤖 Entering Query Mode")
    log.user_info("==================================================")

    try:
        # Load the existing vector store
        embeddings = OllamaEmbeddings(model=EMBEDDING_MODEL)
        vector_store = Chroma(persist_directory=CHROMA_PATH, embedding_function=embeddings)
        retriever = vector_store.as_retriever()

        # Define the prompt template
        template = """Answer the question based only on the following context. If you don't know the answer, just say that you don't know. Don't try to make up an answer.\n\nContext:\n{context}\n\nQuestion: {question}
"""
        prompt = PromptTemplate.from_template(template)

        # We are using phi3 as it is small, fast, and capable.
        # We explicitly set a higher token limit to ensure answers are not cut off.
        ollama_llm = OllamaLLM(model=GENERATION_MODEL, num_predict=1024)

        # Define the RAG chain
        chain = (
            {"context": retriever, "question": RunnablePassthrough()}
            | prompt
            | ollama_llm
            | StrOutputParser()
        )

        # Enter the query loop
        while True:
            question = input("\nAsk a question (or type 'exit' to quit): ")
            if question.lower() == 'exit':
                break
            if not question.strip():
                continue

            # --- Retrieval Step ---
            log.user_info("\n🔍 Retrieving relevant documents...")
            retrieved_docs = retriever.invoke(question)
            log.user_info(f"✅ Found {len(retrieved_docs)} relevant source(s).")
            for doc in retrieved_docs:
                source = doc.metadata.get('source', 'Unknown')
                page = doc.metadata.get('page', 'N/A')
                log.user_info(f"  - {os.path.basename(source)}, Page: {page}")

            # --- Generation Step ---
            context_str = "\n\n".join([doc.page_content for doc in retrieved_docs])
            
            # Setup for heartbeat
            stop_heartbeat = threading.Event()
            heartbeat_thread = threading.Thread(target=heartbeat, args=(stop_heartbeat,))
            
            log.user_info("\n🧠 Synthesizing answer...")
            heartbeat_thread.start()

            # Stream the answer
            full_answer = ""
            for chunk in chain.stream(question):
                if not stop_heartbeat.is_set():
                    stop_heartbeat.set() # Stop the heartbeat on first token
                    heartbeat_thread.join() # Wait for heartbeat to finish
                    print("\r", end="") # Clear the heartbeat line
                
                print(chunk, end="", flush=True)
                full_answer += chunk
            
            # If heartbeat was not stopped (no answer generated)
            if not stop_heartbeat.is_set():
                stop_heartbeat.set()
                heartbeat_thread.join()

            print("\n")

            # --- Save the response to a file ---
            save_query_response(question, retrieved_docs, full_answer)

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    finally:
        print("\nExiting Query Mode.")


def heartbeat(stop_event):
    """Displays a simple spinning cursor as a heartbeat."""
    spinner = itertools.cycle(['|', '/', '-', '\\'])
    while not stop_event.is_set():
        try:
            sys.stdout.write(next(spinner))
            sys.stdout.flush()
            time.sleep(HEARTBEAT_SPEED_SECONDS)
            sys.stdout.write('\r')
        except (IOError, BrokenPipeError):
            # Handle cases where stdout is closed or piped
            break



def process_files(files_to_process, metadata):
    """Process each file, update metadata, and move it to the processed folder."""
    overall_start_time = time.time()
    total_chunks = 0
    total_mb = 0

    for file_path, mod_time in tqdm(files_to_process, desc="Processing Files"):
        file_start_time = time.time()
        file_name = os.path.basename(file_path)
        log.user_info("-" * 50)
        log.user_info(f"Processing file: {file_name}")

        try:
            documents, file_mb = load_documents([(file_path, mod_time)])
            chunks = split_documents(documents)
            add_to_vector_store(chunks)
            
            file_time = time.time() - file_start_time
            total_chunks += len(chunks)
            total_mb += file_mb

            log.info(f"--- File Summary: {file_name} ---")
            log.info(f"  - Size: {file_mb:.2f} MB")
            log.info(f"  - Pages: {len(documents)}")
            log.info(f"  - Chunks: {len(chunks)}")
            log.info(f"  - Time: {file_time:.2f}s")
            log.user_info(f"✅ Finished processing {file_name} (took {file_time:.2f}s).")

            # Update metadata with filename as key
            metadata[file_name] = mod_time
            save_metadata(metadata)

            # Move the processed file
            destination_path = os.path.join(PROCESSED_PATH, file_name)
            shutil.move(file_path, destination_path)
            log.user_info(f"🚚 Moved processed file to: {destination_path}")

        except Exception as e:
            log.error(f"❌ Failed to process {file_name}: {e}")
            # For now, we'll just log the error and continue.

    if len(files_to_process) > 0:
        overall_time = time.time() - overall_start_time
        log.user_info("-" * 50)
        log.user_info("📊 Final Benchmarking Summary 📊")
        log.user_info(f"Total files processed: {len(files_to_process)}")
        log.user_info(f"Total size (MB): {total_mb:.2f}")
        log.user_info(f"Total chunks created: {total_chunks}")
        log.user_info(f"Total processing time: {overall_time:.2f}s")
        if total_mb > 0:
            log.user_info(f"Overall processing speed: {overall_time / total_mb:.2f} s/MB")
        if total_chunks > 0:
            log.user_info(f"Overall embedding speed: {overall_time / total_chunks:.2f} s/chunk")

def clean_database():
    """Deletes the existing database and metadata."""
    log.user_info("🧹 --clean flag detected. Forcing a full rebuild of the database.")
    if os.path.exists(CHROMA_PATH):
        shutil.rmtree(CHROMA_PATH)
        log.info(f"Deleted Chroma DB directory: {CHROMA_PATH}")
    if os.path.exists(METADATA_FILE):
        os.remove(METADATA_FILE)
        log.info(f"Deleted metadata file: {METADATA_FILE}")
    log.user_info("✅ Clean-up complete.")
    log.user_info("🎉 Process complete!")
    log.user_info("==================================================")

def get_files_to_process():
    """Scans the training folder and returns a list of new or modified files."""
    metadata = load_metadata()
    files_to_process = []
    # We only scan the top-level of the training path, not subdirectories
    for file in os.listdir(DOCUMENTS_PATH):
        file_path = os.path.join(DOCUMENTS_PATH, file)
        # Skip directories, process only files
        if not os.path.isfile(file_path):
            continue

        if not (file.endswith(".pdf") or file.endswith(".txt") or file.endswith(".md")):
            log.info(f"Skipping unsupported file type: {file}")
            continue # Skip unsupported file types

        mod_time = os.path.getmtime(file_path)
        # Check metadata using only the filename
        if metadata.get(os.path.basename(file_path)) != mod_time:
            files_to_process.append((file_path, mod_time))
            
    return files_to_process, metadata

def load_documents(file_list):
    """Loads a specific list of files and calculates their total size."""
    log.user_info(f"📚 Loading {len(file_list)} files...")
    loaded_pages = []
    total_mb = 0
    for file_path, _ in file_list:
        try:
            file_size_mb = os.path.getsize(file_path) / (1024 * 1024)
            total_mb += file_size_mb
            log.info(f"  - Loading {os.path.basename(file_path)} ({file_size_mb:.2f} MB)")
            if file_path.endswith(".pdf"):
                loader = PyPDFLoader(file_path)
                loaded_pages.extend(loader.load())
            else:
                loader = TextLoader(file_path, encoding='utf-8')
                loaded_pages.extend(loader.load())
        except Exception as e:
            log.error(f"  - Error loading {os.path.basename(file_path)}: {e}")
    log.user_info(f"✅ Loaded {len(file_list)} files, resulting in {len(loaded_pages)} pages/documents.")
    return loaded_pages, total_mb

def split_documents(documents):
    """Splits documents into smaller chunks."""
    log.user_info("쪼개기 Splitting documents into chunks...")
    text_splitter = RecursiveCharacterTextSplitter(
        chunk_size=1000, chunk_overlap=200, length_function=len, is_separator_regex=False
    )
    chunks = text_splitter.split_documents(documents)
    log.info(f"Split documents into {len(chunks)} chunks.")
    return chunks

def add_to_vector_store(chunks):
    """Creates or updates the Chroma vector store with new chunks."""
    log.user_info("🧠 Creating embeddings and updating vector store...")
    embeddings = OllamaEmbeddings(model=EMBEDDING_MODEL)
    vector_store = Chroma(persist_directory=CHROMA_PATH, embedding_function=embeddings)

    # Add documents in batches with a progress bar
    batch_size = 50
    for i in tqdm(range(0, len(chunks), batch_size), desc="Embedding Chunks"):
        batch = chunks[i:i+batch_size]
        vector_store.add_documents(batch)
    
    log.user_info("✅ Vector store updated successfully.")

def load_metadata():
    """Loads the metadata file that tracks processed files."""
    if os.path.exists(METADATA_FILE):
        with open(METADATA_FILE, 'r') as f:
            return json.load(f)
    return {}

def save_metadata(metadata):
    """Saves the updated metadata to the file."""
    with open(METADATA_FILE, 'w') as f:
        json.dump(metadata, f, indent=4)

def check_embedding_model():
    """Checks if the specified embedding model is available locally."""
    log.info(f"Checking for embedding model: '{EMBEDDING_MODEL}'...")
    try:
        ollama.show(EMBEDDING_MODEL)
        log.info(f"Model '{EMBEDDING_MODEL}' is available.")
    except ollama.ResponseError:
        log.error(f"Model '{EMBEDDING_MODEL}' not found.")
        log.error(f"Please pull it by running: ollama pull {EMBEDDING_MODEL}")
        exit()

if __name__ == "__main__":
    main()