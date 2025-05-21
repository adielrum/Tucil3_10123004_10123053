import tkinter as tk
from tkinter import scrolledtext, messagebox, filedialog
import subprocess
import threading
import os
import re

class RushHourGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Rush Hour Solver")
        self.geometry("800x800")

        # Input area
        tk.Label(self, text="Enter Algorithm (0: UCS, 1: GBFS, 2: A*):").pack(anchor='nw', padx=10, pady=5)
        self.string_input = tk.Entry(self, width=50)
        self.string_input.pack(padx=10, pady=(0, 10))

        tk.Label(self, text="Enter path to .txt file (or use Browse):").pack(anchor='nw', padx=10, pady=(5, 0))
        frame = tk.Frame(self)
        frame.pack(padx=10, pady=(0, 5), fill='x')

        self.file_path_entry = tk.Entry(frame, width=40)
        self.file_path_entry.pack(side='left', expand=True, fill='x')

        browse_btn = tk.Button(frame, text="Browse", command=self.browse_file)
        browse_btn.pack(side='left', padx=(5, 0))

        load_btn = tk.Button(frame, text="Load File", command=self.load_file)
        load_btn.pack(side='left', padx=(5, 0))

        # Puzzle
        tk.Label(self, text="Enter puzzle (N M, num_pieces, then N rows; optional extra K-row):").pack(anchor='nw', padx=10, pady=5)
        self.input_text = scrolledtext.ScrolledText(self, width=50, height=10)
        self.input_text.pack(padx=10)
        self.input_text.insert(tk.END, "6 6\n11\nAAB..F\n..BCDF\nGPPCDFK\nGH.III\nGHJ...\nLLJMM.")

        self.solve_btn = tk.Button(self, text="Solve", command=self.solve)
        self.solve_btn.pack(pady=10)

        # Canvas
        self.cell_size = 40
        self.canvas = tk.Canvas(self, bg='white')
        self.canvas.pack(expand=True, fill='both', padx=10, pady=10)

        self.status = tk.Label(self, text="Status: waiting for input...")
        self.status.pack(anchor='sw', padx=10, pady=5)

        # Data
        self.raw = []
        self.grid = []
        self.moves = []
        self.piece_items = {}
        self.nodes = None
        self.duration = None

    def browse_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
        if file_path:
            self.file_path_entry.delete(0, tk.END)
            self.file_path_entry.insert(0, file_path)

    def load_file(self):
        path = self.file_path_entry.get().strip()
        if not path or not os.path.isfile(path):
            messagebox.showerror("Error", "Invalid file path.")
            return
        try:
            with open(path, 'r') as f:
                content = f.read()
            self.input_text.delete("1.0", tk.END)
            self.input_text.insert(tk.END, content)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to read file:\n{e}")

    def solve(self):
        string_part = self.string_input.get().strip()
        input_data = self.input_text.get("1.0", tk.END).strip()
        
        if not input_data:
            messagebox.showerror("Error", "Please enter puzzle input.")
            return

        # Combine the inputs — modify this logic to suit your format
        combined_input = string_part + "\n" + input_data if string_part else input_data

        self.solve_btn.config(state=tk.DISABLED)
        self.status.config(text="Status: solving...")
        threading.Thread(target=self.run_solver, args=(combined_input,)).start()

    def run_solver(self, input_data):
        # 1) Read & sanitize raw lines
        lines = input_data.splitlines()
        # print(lines)
        algo = map(int, lines[0].split())
        N, M = map(int, lines[1].split())
        raw = lines[3:3+N]
        # if no K found, grab the extra line
        if (all('K' not in row for row in raw) and len(lines) > 3+N) or ('K' in raw[0]):
            raw.append(lines[3+N])

        def print_Grid(Grid):
            for i in range(len(Grid)):
                for j in range(len(Grid[i])):
                    print(Grid[i][j],end=" ")
                print()

        # print_Grid(raw)
        # 2) Locate raw K cell and detect empty‐row condition
        raw_exit_x = raw_exit_y = None
        empty_row = False
        for i, row in enumerate(raw):
            if 'K' in row:
                raw_exit_x, raw_exit_y = i, row.index('K')
                if(raw_exit_y == 0):
                    for j, rew in enumerate(raw):
                        if i != j:
                            rews = rew.replace(' ', '', 1)
                            raw[j] = rews
                row = row.replace('K', '', 1)
                raw[i] = row
                # print(raw_exit_x)
                # check if every other character is space (or '.') 
                if raw_exit_x == 0 or raw_exit_y == len(raw):
                    empty_row = True
                break
        
        # print_Grid(raw)

        # 3) If it was an empty row, delete it
        if empty_row and raw_exit_x in (0, len(raw)-1):
            del raw[raw_exit_x]
            # adjust raw_exit_x if it was the bottom row
            if raw_exit_x == len(raw):
                raw_exit_x = None  # will reset below

        # 4) Build sanitized grid (replace K with '.')
        sanitized = [ (r.replace('K','.') + '.'*M)[:M] for r in raw ]
        # print_Grid(sanitized)
        # 5) Compute padded exit_x,exit_y
        # after deletion, raw_exit_x may have changed if bottom
        if raw_exit_x is None:
            # bottom‐edge exit: now at deleted‐row index
            raw_exit_x = len(sanitized)
        # four edge cases:
        if raw_exit_x == 0 and empty_row:
            # top edge
            exit_x, exit_y = 0, raw_exit_y+1
        elif raw_exit_x == len(sanitized) and empty_row:
            # bottom edge
            exit_x, exit_y = raw_exit_x+1, raw_exit_y+1
        elif raw_exit_y == 0:
            # left edge
            exit_x, exit_y = raw_exit_x+1, 0
        elif raw_exit_y == M-1:
            # right edge
            exit_x, exit_y = raw_exit_x+1, raw_exit_y+1
        else:
            # interior K (shouldn’t happen)
            exit_x = raw_exit_x+1
            exit_y = raw_exit_y+1

        # 6) Build the padded grid with sentinels
        H = len(sanitized)
        W = M
        padded = [['*']*(W+2) for _ in range(H+2)]
        for i in range(H):
            for j in range(W):
                padded[i+1][j+1] = sanitized[i][j]

        # 7) Overwrite the exit cell to '.' so we can slide through
        padded[exit_x][exit_y] = '.'

        # Save into self
        self.raw = [list(r) for r in sanitized]
        self.grid = padded
        self.rows, self.cols = H+2, W+2
        self.exit_pos = (exit_x, exit_y)

        # Run executable to get moves
        base_dir = os.path.dirname(__file__)
        exe_path = os.path.normpath(os.path.join(base_dir, '..', 'bin', 'gui.exe'))
        try:
            proc = subprocess.run([exe_path], input=input_data, capture_output=True, text=True)
        except Exception as e:
            self.after(0, lambda: messagebox.showerror("Error", str(e)))
            self.after(0, lambda: self.solve_btn.config(state=tk.NORMAL))
            return

        # Parse moves from stdout
        self.moves = []
        nodes = None
        duration = None

        for line in proc.stdout.splitlines():
            if "Dikunjungi" in line and "ms" in line:
                match = re.search(r"Dikunjungi (\d+) simpul dalam ([\d.]+) ms", line)
                if match:
                    nodes = int(match.group(1))
                    duration = float(match.group(2))
            m = re.match(r"Move \d+: Piece (\w) moves (\w+) by (\d+)", line)
            if m:
                name, direction, dist = m.group(1), m.group(2), int(m.group(3))
                self.moves.append((name, direction, dist))
        
        self.nodes = nodes
        self.duration = duration

        # Draw and animate
        self.after(0, self.draw_background)
        self.after(0, self.draw_pieces)
        self.after(500, self.animate_next_move)
        status_msg = f"Status: {len(self.moves)} moves loaded"
        if nodes is not None and duration is not None:
            status_msg += f" | Simpul: {nodes} | Waktu: {duration} ms"

        self.after(0, lambda: self.status.config(text=status_msg))

    def draw_background(self):
        self.canvas.delete('all')
        w, h = self.cols*self.cell_size, self.rows*self.cell_size
        self.canvas.config(width=w, height=h)
        for i in range(self.rows):
            for j in range(self.cols):
                x0, y0 = j*self.cell_size, i*self.cell_size
                x1, y1 = x0+self.cell_size, y0+self.cell_size
                ch = self.grid[i][j]
                fill = 'darkgrey' if ch=='*' else 'lightgrey'
                self.canvas.create_rectangle(x0, y0, x1, y1, fill=fill, outline='black')

    def draw_pieces(self):
        for ids in self.piece_items.values():
            for _id in ids: self.canvas.delete(_id)
        self.piece_items.clear()
        for i in range(self.rows):
            for j in range(self.cols):
                ch = self.grid[i][j]
                if ch not in ('.','*',' '):
                    x0, y0 = j*self.cell_size, i*self.cell_size
                    x1, y1 = x0+self.cell_size, y0+self.cell_size
                    fill = 'red' if ch=='P' else 'skyblue'
                    rect = self.canvas.create_rectangle(x0, y0, x1, y1, fill=fill, outline='black', tags=(ch,))
                    text = self.canvas.create_text((x0+x1)/2, (y0+y1)/2, text=ch, font=('Arial',14,'bold'), tags=(ch,))
                    self.piece_items.setdefault(ch, []).extend([rect,text])

    def animate_next_move(self):
        if not self.moves:
            extra_info = ""
            if self.nodes is not None and self.duration is not None:
                extra_info = f" | Simpul: {self.nodes} | Waktu: {self.duration} ms"
            self.status.config(text=f"Animation complete{extra_info}")
            self.solve_btn.config(state=tk.NORMAL)
            return
        name, direction, dist = self.moves.pop(0)
        extra_info = ""
        if self.nodes is not None and self.duration is not None:
            extra_info = f" | Simpul: {self.nodes} | Waktu: {self.duration} ms"
        self.status.config(text=f"Moving {name} {direction} by {dist}{extra_info}")
        dx = (direction=='kanan') - (direction=='kiri')
        dy = (direction=='bawah') - (direction=='atas')
        dx *= self.cell_size; dy *= self.cell_size
        total_dx = dx//self.cell_size * dist
        total_dy = dy//self.cell_size * dist

        def step(k):
            if k==0:
                new = [row.copy() for row in self.grid]
                for i in range(self.rows):
                    for j in range(self.cols):
                        if self.grid[i][j]==name: new[i][j]='.'
                for i in range(self.rows):
                    for j in range(self.cols):
                        if self.grid[i][j]==name:
                            ni, nj = i+total_dy, j+total_dx
                            if 0<=ni<self.rows and 0<=nj<self.cols:
                                new[ni][nj]=name
                self.grid=new; self.draw_pieces()
                self.after(300, self.animate_next_move)
            else:
                for item in self.piece_items.get(name,[]):
                    self.canvas.move(item, dx, dy)
                self.after(150, lambda: step(k-1))
        step(dist)

if __name__=='__main__':
    RushHourGUI().mainloop()