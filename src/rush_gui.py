import tkinter as tk
from tkinter import scrolledtext, messagebox
import subprocess
import threading

class RushHourGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Rush Hour Solver")
        self.geometry("600x650")

        # Input area
        tk.Label(self, text="Puzzle Input (N M, num_pieces, then N rows):").pack(anchor='nw')
        self.input_text = scrolledtext.ScrolledText(self, width=40, height=8)
        self.input_text.pack()
        self.input_text.insert(tk.END, "6 6\n11\nAAB..F\n..BCDF\nGPPCDFK\nGH.III\nGHJ...\nLLJMM.")

        self.solve_btn = tk.Button(self, text="Solve", command=self.solve)
        self.solve_btn.pack(pady=5)

        # Canvas for board
        self.cell_size = 50
        self.canvas = tk.Canvas(self, width=self.cell_size*6, height=self.cell_size*6, bg='white')
        self.canvas.pack(pady=10)

        self.status = tk.Label(self, text="Status: waiting for input...")
        self.status.pack(anchor='sw')

        # Data
        self.grid = []
        self.moves = []
        self.piece_items = {}  # name -> list of canvas IDs

    def solve(self):
        input_data = self.input_text.get("1.0", tk.END).strip()
        if not input_data:
            messagebox.showerror("Error", "Please enter puzzle input.")
            return
        self.status.config(text="Status: solving...")
        self.solve_btn.config(state=tk.DISABLED)
        threading.Thread(target=self.run_solver, args=(input_data,)).start()

    def run_solver(self, input_data):
        try:
            proc = subprocess.run(['../bin/main.exe'], input=input_data.encode(), capture_output=True)
            output_lines = proc.stdout.decode().splitlines()
        except Exception as e:
            self.after(0, lambda: self.status.config(text=f"Error: {e}"))
            self.after(0, lambda: self.solve_btn.config(state=tk.NORMAL))
            return

        # Parse moves
        self.moves = []
        for line in output_lines:
            if line.startswith("Move "):
                parts = line.split()
                name = parts[3]
                direction = parts[5]
                dist = int(parts[7])
                self.moves.append((name, direction, dist))

        # Initialize grid and draw
        lines = input_data.splitlines()
        self.grid = [list(row) for row in lines[2:]]
        self.N = len(self.grid)
        self.M = len(self.grid[0])

        self.after(0, self.draw_background)
        self.after(0, self.draw_pieces)
        self.after(500, self.animate_next_move)
        self.after(0, lambda: self.status.config(text=f"Status: {len(self.moves)} moves loaded"))

    def draw_background(self):
        self.canvas.delete('all')
        for i in range(self.N):
            for j in range(self.M):
                x0 = j * self.cell_size
                y0 = i * self.cell_size
                x1 = x0 + self.cell_size
                y1 = y0 + self.cell_size
                self.canvas.create_rectangle(x0, y0, x1, y1, outline='black', fill='lightgrey')

    def draw_pieces(self):
        # Clear old piece items
        for ids in self.piece_items.values():
            for _id in ids:
                self.canvas.delete(_id)
        self.piece_items.clear()

        # Draw each piece cell with color (P in red)
        for i in range(self.N):
            for j in range(self.M):
                ch = self.grid[i][j]
                if ch != '.':
                    x0 = j * self.cell_size
                    y0 = i * self.cell_size
                    x1 = x0 + self.cell_size
                    y1 = y0 + self.cell_size
                    fill_color = 'red' if ch == 'P' else 'skyblue'
                    rect = self.canvas.create_rectangle(x0, y0, x1, y1, fill=fill_color, outline='black', tags=(ch,))
                    text = self.canvas.create_text((x0+x1)/2, (y0+y1)/2, text=ch, font=('Arial', 16, 'bold'), tags=(ch,))
                    self.piece_items.setdefault(ch, []).extend([rect, text])

    def animate_next_move(self):
        if not self.moves:
            self.status.config(text="Status: animation complete")
            self.solve_btn.config(state=tk.NORMAL)
            return

        name, direction, dist = self.moves.pop(0)
        self.status.config(text=f"Moving {name} {direction} by {dist}")

        dx, dy = 0, 0
        if direction == 'kanan': dx = self.cell_size
        elif direction == 'kiri': dx = -self.cell_size
        elif direction == 'bawah': dy = self.cell_size
        elif direction == 'atas': dy = -self.cell_size

        total_cell_dx = dx // self.cell_size * dist
        total_cell_dy = dy // self.cell_size * dist

        def animate_unit(steps_left):
            if steps_left == 0:
                # Update grid array: move letter positions by full dist
                new_grid = [row.copy() for row in self.grid]
                # Clear old positions
                for i in range(self.N):
                    for j in range(self.M):
                        if new_grid[i][j] == name:
                            new_grid[i][j] = '.'
                # Place at new cells, skip out-of-bounds for exit
                for i in range(self.N):
                    for j in range(self.M):
                        if self.grid[i][j] == name:
                            new_i = i + total_cell_dy
                            new_j = j + total_cell_dx
                            if 0 <= new_i < self.N and 0 <= new_j < self.M:
                                new_grid[new_i][new_j] = name
                self.grid = new_grid
                self.draw_pieces()
                self.after(300, self.animate_next_move)
                return

            # Move canvas items by one cell
            for item_id in self.piece_items.get(name, []):
                self.canvas.move(item_id, dx, dy)
            self.after(150, lambda: animate_unit(steps_left-1))

        animate_unit(dist)

if __name__ == '__main__':
    app = RushHourGUI()
    app.mainloop()