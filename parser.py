import io

def main():
    with open('index.html', 'r', encoding='utf-8', newline='') as source:
        text = source.read().split('\n')
        print(len(text))

        with io.open('out.txt', 'w', encoding='utf-8') as file:
            file.write('String html_body = \n')
            
            for line in text:
                line = f"\"{line[:-1]}\"\n"
                file.write(line)

if __name__ == '__main__':
    main()