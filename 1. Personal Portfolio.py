class personal_info:
    def __init__(self):
        self.name = input("Enter your name: ")
        self.phone = input("Enter your phone number: ")
        self.email = input("Enter your email: ")
        self.address = input("Enter your address: ")
    
    def print_data(self):
        print(f"your name is: {self.name}")
        print(f"your phone is: {self.phone}")
        print(f"your email is: {self.email}")
        print(f"your address is: {self.address}")

class education(personal_info):
    def __init__(self):
        super().__init__()
        self.matric = self.edu()
        self.FSC = self.edu()
        self.BS = self.edu()
        self.training = self.edu()
    class edu:
        def __init__(self):
            self.Title = input("Enter your Title: ")
            self.major = input("Enter your major: ")
            self.institute = input("Enter your intitute name: ")
            self.session = input("Enter your session: ")
    
    def displaying_edu(self):
        print(f"Matriculation: {self.matric.Title} in {self.matric.major} from {self.matric.institute} in {self.matric.session}")
        print(f"FSc: {self.FSC.Title} in {self.FSC.major} from {self.FSC.institute} in {self.FSC.session}")
        print(f"BS is: {self.BS.Title} in {self.BS.major} from {self.BS.institute} in {self.BS.session}")
        print(f"Training: {self.training.Title} in {self.training.major} from {self.training.institute} in {self.training.session}")

class experience(education):
    def __init__(self):
        super().__init__()
        self.internship = self.exp()
        self.experience1 = self.exp()
    class exp:
        def __init__(self):
            self.title = input("enter job tital: ")
            self.dated = input("duration: ")
            self.company = input("enter previous company name: ")
    
    def display_exp(self):
        print(f"Internship: {self.internship.title} at {self.internship.company} during {self.internship.dated}")
        print(f"Work Experience: {self.experience1.title} at {self.experience1.company} during {self.experience1.dated}")

ra = experience()
ra.print_data()
ra.displaying_edu()
ra.display_exp()